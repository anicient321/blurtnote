#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "themeManager.h"
#include "documentManager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLayout>
#include <QTextEdit>
#include <QActionGroup>
#include <QRegularExpression>
#include <QDebug>
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    
    ui->setupUi(this);

    // Layout margins (set programmatically to avoid uic schema issues)
    if (auto lay = qobject_cast<QLayout*>(ui->centralwidget->layout()))
        lay->setContentsMargins(12, 12, 12, 12);

    // sets check for currently active theme(when opening app)
    updateMenuChecks();

    // make theme actions mutually exclusive
    {
        auto *group = new QActionGroup(this);
        group->setExclusive(true);
        group->addAction(ui->actionLight_mode);
        group->addAction(ui->actionDark_mode);
    }

    // create first tab and document/manager
    m_counterLabel = new QLabel(this);
    m_counterLabel->setText("");
    m_counterLabel->setVisible(false);
    m_counterLabel->setObjectName("counterLabel");
    m_counterLabel->setStyleSheet("font-size:10px; padding-left:6px; padding-right:6px;");
    statusBar()->addWidget(m_counterLabel);

    // create initial tab
    createTab("Untitled");

    // connect menu actions
    connect(ui->actionNew_File, &QAction::triggered,
            this, &MainWindow::openFile);
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::newFile);
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);
    connect(ui->actionSave_as, &QAction::triggered,
            this, &MainWindow::saveFileAs);
    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::close);
    connect(ui->actionAbout, &QAction::triggered,
            this, &MainWindow::showAbout);

    // wire toggle counter action
    connect(ui->actionToggle_Counter, &QAction::toggled,
            this, &MainWindow::toggleCounter);
    ui->actionToggle_Counter->setChecked(false);

    // tab widget signals
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::onTabChanged);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::closeTabRequested);

    // cycling actions
    connect(ui->actionNext_Tab, &QAction::triggered, this, &MainWindow::nextTab);
    connect(ui->actionPrev_Tab, &QAction::triggered, this, &MainWindow::prevTab);

    // autosave timer and controls
    m_autosaveTimer = new QTimer(this);
    m_autosaveTimer->setInterval(500); // 500ms autosave
    connect(m_autosaveTimer, &QTimer::timeout,
            this, &MainWindow::onAutosaveTick);

    connect(ui->actionAutosave, &QAction::toggled,
            this, &MainWindow::toggleAutosave);

    // restore saved autosave state
    {
        QSettings settings;
        bool autosaveEnabled = settings.value("autosaveEnabled", false).toBool();
        ui->actionAutosave->setChecked(autosaveEnabled);
        toggleAutosave(autosaveEnabled);
    }

    // opening the last used file
    const QString path = lastOpenFile();

    if (!path.isEmpty() && QFileInfo::exists(path)) {
        createTab(QFileInfo(path).fileName(), path);
        setLastDirectory(path);
    }

    // sets window title as document name
    updateWindowTitle();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------THEMES------------------------------------------

void MainWindow::updateMenuChecks()
{
    auto current = ThemeManager::instance().currentTheme();

    ui->actionLight_mode->setChecked(current == ThemeManager::Theme::Light);
    ui->actionDark_mode->setChecked(current == ThemeManager::Theme::Dark);
}

// light mode toggle
void MainWindow::on_actionLight_mode_toggled(bool checked)
{
    qDebug() << "blurtnote: + light mode toggled:" << checked;
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Light);
        updateMenuChecks();
        qDebug() << "blurtnote: + light theme applied";
    }
}

// dark mode toggle
void MainWindow::on_actionDark_mode_toggled(bool checked)
{
    qDebug() << "blurtnote: + dark mode toggled:" << checked;
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Dark);
        updateMenuChecks();
        qDebug() << "blurtnote: + dark theme applied";
    }
}

//-------------------------FILE HANDLING---------------------

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        getLastDirectory(),
        tr("Text Files (*.txt);;Rich Text Files (*.rtf);;All Files (*)")
        );

    if (path.isEmpty())
        return;

    createTab(QFileInfo(path).fileName(), path);
    setLastDirectory(path);
}

void MainWindow::saveFile()
{
    DocumentManager* dm = currentDocManager();
    if (!dm)
        return;

    if (!dm->hasFile()) {
        saveFileAs();
        return;
    }

    dm->save();
    updateWindowTitle();
} 

void MainWindow::saveFileAs()
{
    DocumentManager* dm = currentDocManager();
    if (!dm) return;

    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectory(getLastDirectory());
    dialog.setNameFilters({
        "Text Files (*.txt)",
        "Rich Text Files (*.rtf)",
        "All Files (*)"
    });
    dialog.setDefaultSuffix("txt");

    if (!dialog.exec())
        return;

    const QStringList files = dialog.selectedFiles();
    if (files.isEmpty())
        return;

    QString path = files.first();

    if (dm->saveAs(path)) {
        setLastDirectory(path);
        updateWindowTitle();
        // update tab label to file name
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(path).fileName());
    }
} 

void MainWindow::newFile()
{
    // Create a new untitled tab and switch to it
    createTab("Untitled");
} 

//updates window title to name of file
void MainWindow::updateWindowTitle()
{
    QString title = "BlurtNote";

    DocumentManager* dm = currentDocManager();
    QTextDocument* doc = currentDocument();
    if (dm && dm->hasFile()) {
        title = QFileInfo(dm->filePath()).fileName();
    } else if (ui->tabWidget->count() > 0) {
        // use tab text as document name when untitled
        title = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    }

    if (doc && doc->isModified())
        title += " *";

    setWindowTitle(title);
} 

//saves last directory used for files
QString MainWindow::getLastDirectory() const
{
    QSettings settings;
    return settings.value("lastDir", QDir::homePath()).toString();
}

void MainWindow::setLastDirectory(const QString& path)
{
    QSettings settings;
    settings.setValue("lastDir", QFileInfo(path).absolutePath());
}

//Last open file open and save
void MainWindow::rememberLastOpenFile()
{
    QSettings settings;
    DocumentManager* dm = currentDocManager();
    if (dm && dm->hasFile()) {
        settings.setValue("lastOpenFile", dm->filePath());
    } else {
        settings.remove("lastOpenFile");
    }
} 

QString MainWindow::lastOpenFile() const
{
    QSettings settings;
    return settings.value("lastOpenFile").toString();
}

//Saving dialog when exiting without saving changes

void MainWindow::closeEvent(QCloseEvent *event)
{
    // If no modified documents, accept and remember last open file
    bool anyModified = false;
    for (QTextDocument* d : qAsConst(m_documents)) {
        if (d && d->isModified()) { anyModified = true; break; }
    }

    if (!anyModified) {
        rememberLastOpenFile();
        event->accept();
        return;
    }

    // Ask user per modified tab
    for (int i = 0; i < m_documents.size(); ++i) {
        QTextDocument* d = m_documents.at(i);
        if (!d || !d->isModified()) continue;

        ui->tabWidget->setCurrentIndex(i);
        DocumentManager* dm = m_docManagers.at(i);
        const QString name = dm && dm->hasFile() ? QFileInfo(dm->filePath()).fileName() : ui->tabWidget->tabText(i);

        const QMessageBox::StandardButton result =
            QMessageBox::question(
                this,
                tr("Unsaved Changes"),
                tr("'%1' has unsaved changes. Save before exiting?").arg(name),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                QMessageBox::Save);

        if (result == QMessageBox::Cancel) {
            event->ignore();
            return;
        }

        if (result == QMessageBox::Save) {
            // Save or Save As if needed
            if (dm) {
                if (dm->hasFile()) {
                    if (!dm->save()) { event->ignore(); return; }
                } else {
                    QFileDialog dialog(this);
                    dialog.setAcceptMode(QFileDialog::AcceptSave);
                    dialog.setDirectory(getLastDirectory());
                    dialog.setNameFilters({"Text Files (*.txt)", "Rich Text Files (*.rtf)", "All Files (*)"});
                    dialog.setDefaultSuffix("txt");
                    if (!dialog.exec()) { event->ignore(); return; }
                    const QStringList files = dialog.selectedFiles();
                    if (files.isEmpty()) { event->ignore(); return; }
                    if (!dm->saveAs(files.first())) { event->ignore(); return; }
                }
            }
        }
        // Discard -> continue to next modified
    }

    // All handled
    rememberLastOpenFile();
    event->accept();
} 

void MainWindow::showAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::toggleCounter(bool checked)
{
    m_counterLabel->setVisible(checked);
    if (checked)
        updateCounter();
}

void MainWindow::updateCounter()
{
    if (!m_counterLabel || !m_counterLabel->isVisible())
        return;

    QTextDocument* doc = currentDocument();
    if (!doc) return;
    const QString text = doc->toPlainText();

    // Word count: find word-like tokens using Unicode-aware regex
    QRegularExpression wordRe("\\b[\\p{L}0-9_'-]+\\b");
    int words = 0;
    if (!text.trimmed().isEmpty()) {
        auto it = wordRe.globalMatch(text);
        while (it.hasNext()) { it.next(); ++words; }
    }

    // Sentence count: prefer explicit sentence-ending punctuation, fallback to non-empty lines
    QRegularExpression sentenceRe("[.!?]+(?=\\s|$)");
    int sentences = 0;
    if (!text.isEmpty()) {
        auto it2 = sentenceRe.globalMatch(text);
        while (it2.hasNext()) { it2.next(); ++sentences; }

        if (sentences == 0) {
            // fallback: count non-empty lines as sentences
            const QStringList lines = text.split('\n', Qt::SkipEmptyParts);
            int nonEmpty = 0;
            for (const QString &l : lines) {
                if (!l.trimmed().isEmpty()) ++nonEmpty;
            }
            sentences = nonEmpty;
        }
    }

    m_counterLabel->setText(QString("W: %1  S: %2").arg(words).arg(sentences));
}

void MainWindow::toggleAutosave(bool checked)
{
    QSettings settings;
    settings.setValue("autosaveEnabled", checked);

    if (checked) {
        if (!m_autosaveTimer->isActive())
            m_autosaveTimer->start();
    } else {
        if (m_autosaveTimer->isActive())
            m_autosaveTimer->stop();
    }
}

QTextDocument* MainWindow::currentDocument() const
{
    const int idx = ui->tabWidget->currentIndex();
    if (idx < 0 || idx >= m_documents.size()) return nullptr;
    return m_documents.at(idx);
}

DocumentManager* MainWindow::currentDocManager() const
{
    const int idx = ui->tabWidget->currentIndex();
    if (idx < 0 || idx >= m_docManagers.size()) return nullptr;
    return m_docManagers.at(idx);
}

void MainWindow::createTab(const QString &title, const QString &path)
{
    QTextEdit* editor = new QTextEdit;
    QTextDocument* doc = new QTextDocument(this);
    editor->setDocument(doc);
    DocumentManager* dm = new DocumentManager(doc, this);

    m_documents.append(doc);
    m_docManagers.append(dm);

    int idx = ui->tabWidget->addTab(editor, title.isEmpty() ? "Untitled" : title);
    ui->tabWidget->setCurrentIndex(idx);

    // connect signals for this document
    connect(doc, &QTextDocument::modificationChanged, this, &MainWindow::updateWindowTitle);
    connect(doc, &QTextDocument::contentsChanged, this, &MainWindow::updateCounter);

    if (!path.isEmpty()) {
        if (dm->openFile(path)) {
            ui->tabWidget->setTabText(idx, QFileInfo(path).fileName());
        } else {
            QMessageBox::warning(this, tr("Open Failed"), tr("Failed to open %1").arg(path));
        }
    }

    updateWindowTitle();
    updateCounter();
}

void MainWindow::onTabChanged(int index)
{
    Q_UNUSED(index);
    updateWindowTitle();
    updateCounter();
}

void MainWindow::closeTabRequested(int index)
{
    if (index < 0 || index >= m_documents.size()) return;

    QTextDocument* d = m_documents.at(index);
    DocumentManager* dm = m_docManagers.at(index);

    if (d && d->isModified()) {
        const QString name = dm && dm->hasFile() ? QFileInfo(dm->filePath()).fileName() : ui->tabWidget->tabText(index);
        const QMessageBox::StandardButton result =
            QMessageBox::question(this, tr("Unsaved Changes"), tr("'%1' has unsaved changes. Save?").arg(name),
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if (result == QMessageBox::Cancel) return;
        if (result == QMessageBox::Save) {
            if (dm) {
                if (dm->hasFile()) {
                    if (!dm->save()) return; // abort close on failure
                } else {
                    // ask for path
                    QFileDialog dialog(this);
                    dialog.setAcceptMode(QFileDialog::AcceptSave);
                    dialog.setDirectory(getLastDirectory());
                    dialog.setNameFilters({"Text Files (*.txt)", "Rich Text Files (*.rtf)", "All Files (*)"});
                    dialog.setDefaultSuffix("txt");
                    if (!dialog.exec()) return;
                    const QStringList files = dialog.selectedFiles();
                    if (files.isEmpty()) return;
                    if (!dm->saveAs(files.first())) return;
                }
            }
        }
    }

    QWidget* w = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    if (w) delete w;

    // clean up vectors
    delete m_documents.takeAt(index);
    delete m_docManagers.takeAt(index);

    if (ui->tabWidget->count() == 0) {
        createTab("Untitled");
    } else {
        updateWindowTitle();
        updateCounter();
    }
}

void MainWindow::nextTab()
{
    int cnt = ui->tabWidget->count();
    if (cnt <= 1) return;
    int i = ui->tabWidget->currentIndex();
    ui->tabWidget->setCurrentIndex((i + 1) % cnt);
}

void MainWindow::prevTab()
{
    int cnt = ui->tabWidget->count();
    if (cnt <= 1) return;
    int i = ui->tabWidget->currentIndex();
    ui->tabWidget->setCurrentIndex((i - 1 + cnt) % cnt);
}

void MainWindow::onAutosaveTick()
{
    // Only autosave when the action is checked
    if (!ui->actionAutosave->isChecked())
        return;

    for (int i=0;i<m_docManagers.size();++i) {
        DocumentManager* dm = m_docManagers.at(i);
        QTextDocument* d = m_documents.at(i);
        if (!dm || !d) continue;
        if (d->isModified() && dm->hasFile()) {
            dm->save();
        }
    }

    // refresh title for current tab
    updateWindowTitle();
}
