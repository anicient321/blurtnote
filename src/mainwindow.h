#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextDocument>
#include <QTimer>
#include <QLabel>
#include "documentManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLight_mode_toggled(bool checked);
    void on_actionDark_mode_toggled(bool checked);

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void showAbout();
    void toggleAutosave(bool checked);
    void onAutosaveTick();
    void toggleCounter(bool checked);
    void updateCounter();

    // Tabs
    void onTabChanged(int index);
    void closeTabRequested(int index);
    void nextTab();
    void prevTab();
    void createTab(const QString &title = QString(), const QString &path = QString());

private:
    void updateWindowTitle();
    QString getLastDirectory() const;
    void setLastDirectory(const QString& path);

    void rememberLastOpenFile();
    QString lastOpenFile() const;

    // Per-tab documents and managers
    QVector<QTextDocument*> m_documents;
    QVector<DocumentManager*> m_docManagers;

private:
    QTimer *m_autosaveTimer;
    QLabel *m_counterLabel;

    Ui::MainWindow *ui;

    // Helpers
    QTextDocument* currentDocument() const;
    DocumentManager* currentDocManager() const;

    void updateMenuChecks();



protected:
     void closeEvent(QCloseEvent *event) override;
};

#endif
