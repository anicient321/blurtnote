#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextDocument>
#include "DocumentManager.h"

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

    void openFile();
    void saveFile();
    void saveFileAs();

private:
    void updateWindowTitle();
    QString getLastDirectory() const;
    void setLastDirectory(const QString& path);

    void rememberLastOpenFile();
    QString lastOpenFile() const;

    QTextDocument *document;
    DocumentManager *docManager;

private:

    Ui::MainWindow *ui;

    void updateMenuChecks();



protected:
     void closeEvent(QCloseEvent *event) override;
};

#endif
