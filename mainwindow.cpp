#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thememanager.h"
#include "documentManager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // sets check for currently active theme(when opening app)
    updateMenuChecks();

    // Document creation
    document = new QTextDocument(this);

    // Attach document to editor
    ui->textEdit->setDocument(document);

    // Create document manager
    docManager = new DocumentManager(document, this);

    // Connect menu actions
    connect(ui->actionNew_File, &QAction::triggered,
            this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);
    connect(ui->actionSave_as, &QAction::triggered,
            this, &MainWindow::saveFileAs);

    connect(document, &QTextDocument::modificationChanged,
            this, &MainWindow::updateWindowTitle);

    //Sets window title as document name
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
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Light);
        updateMenuChecks();
    }
}

// dark mode toggle
void MainWindow::on_actionDark_mode_toggled(bool checked)
{
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Dark);
        updateMenuChecks();
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

    if (docManager->openFile(path)) {
        setLastDirectory(path);
        updateWindowTitle();
    }
}

void MainWindow::saveFile()
{
    if (!docManager->hasFile()) {
        saveFileAs();
        return;
    }

    docManager->save();
    updateWindowTitle();
}

void MainWindow::saveFileAs()
{
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


    if (docManager->saveAs(path)) {
        setLastDirectory(path);
        updateWindowTitle();
    }
}

//updates window title to name of file
void MainWindow::updateWindowTitle()
{
    QString title = "Qt Notes";

    if (docManager->hasFile()) {
        title = QFileInfo(docManager->filePath()).fileName();
    }

    if (document->isModified())
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

//Saving dialog when exiting without saving changes

void MainWindow::closeEvent(QCloseEvent *event)
{
    //No unsaved changes -> close
    if (!document->isModified()) {
        event->accept();
        return;
    }

    //Ask the user
    const QMessageBox::StandardButton result =
        QMessageBox::question(
            this,
            tr("Unsaved Changes"),
            tr("The document has unsaved changes.\nDo you want to save them before closing?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save
            );

    //User canceled close
    if (result == QMessageBox::Cancel) {
        event->ignore();
        return;
    }

    //User chose Discard
    if (result == QMessageBox::Discard) {
        event->accept();
        return;
    }

    //User chose Save
    if (result == QMessageBox::Save) {

        // 5a. File already has a path → save directly
        if (docManager->hasFile()) {
            if (docManager->save()) {
                event->accept();
            } else {
                event->ignore();
            }
            return;
        }

        //No path yet -> Save As dialog
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDirectory(getLastDirectory());
        dialog.setNameFilters({
            "Text Files (*.txt)",
            "Rich Text Files (*.rtf)",
            "All Files (*)"
        });
        dialog.setDefaultSuffix("txt");

        if (!dialog.exec()) {
            //User canceled Save As -> cancel close
            event->ignore();
            return;
        }

        const QStringList files = dialog.selectedFiles();
        if (files.isEmpty()) {
            event->ignore();
            return;
        }

        const QString path = files.first();

        if (docManager->saveAs(path)) {
            setLastDirectory(path);
            event->accept();
        } else {
            event->ignore();
        }
    }
}
