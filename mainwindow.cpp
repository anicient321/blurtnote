#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thememanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // postavi check-e prema trenutno aktivnoj temi
    updateMenuChecks();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMenuChecks()
{
    auto current = ThemeManager::instance().currentTheme();

    ui->actionLight_mode->setChecked(current == ThemeManager::Theme::Light);
    ui->actionDark_mode->setChecked(current == ThemeManager::Theme::Dark);
}

// slot za light mode toggle
void MainWindow::on_actionLight_mode_toggled(bool checked)
{
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Light);
        updateMenuChecks();
    }
}

// slot za dark mode toggle
void MainWindow::on_actionDark_mode_toggled(bool checked)
{
    if (checked) {
        ThemeManager::instance().applyTheme(ThemeManager::Theme::Dark);
        updateMenuChecks();
    }
}
