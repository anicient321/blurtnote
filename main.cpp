#include "mainwindow.h"
#include <QApplication>
#include "themeManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setApplicationName("MyQtApp");

    // učitaj spremljenu temu prije otvaranja prozora
    ThemeManager::instance().loadThemeOnStartup();

    MainWindow w;
    w.show();

    return app.exec();
}
