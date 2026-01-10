#include "thememanager.h"
#include <QApplication>
#include <QSettings>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

// Konstruktor
ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent),
    m_currentTheme(Theme::Light)   // default theme
{
    loadThemeOnStartup();            // load saved theme
}

// return current active theme
ThemeManager::Theme ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

// apply theme and save
void ThemeManager::applyTheme(Theme theme)
{
    if (theme == m_currentTheme)
        return;

    m_currentTheme = theme;

    switch (theme) {
    case Theme::Light:
        applyLightTheme();
        break;
    case Theme::Dark:
        applyDarkTheme();
        break;
    }

    saveCurrentTheme();
}

// Load theme form Qsettigns on startup
void ThemeManager::loadThemeOnStartup()
{
    QSettings settings;
    int saved = settings.value("theme", static_cast<int>(Theme::Light)).toInt();
    Theme loadedTheme = static_cast<Theme>(saved);
    m_currentTheme = loadedTheme;

    // primijeni odmah
    switch (m_currentTheme) {
    case Theme::Light:
        applyLightTheme();
        break;
    case Theme::Dark:
        applyDarkTheme();
        break;
    }
}

//save current theme in Qsettings
void ThemeManager::saveCurrentTheme()
{
    QSettings settings;
    settings.setValue("theme", static_cast<int>(m_currentTheme));
}

// Hard-coded light theme
void ThemeManager::applyLightTheme()
{
    QString style = R"(
        QMainWindow {
            background-color: #ffffff;
        }
        QWidget {
            color: #000000;
            background-color: #ffffff;
            font-family: "Segoe UI";
            font-size: 10pt;
        }
        QMenuBar {
            background-color: #f2f2f2;
            color: #000000;
        }
        QMenuBar::item:selected {
            background-color: #dcdcdc;
        }
        QMenu {
            background-color: #ffffff;
            color: #000000;
        }
        QMenu::item:selected {
            background-color: #e6e6e6;
        }
        QPushButton {
            background-color: #f5f5f5;
            border: 1px solid #c8c8c8;
            padding: 6px;
        }
        QPushButton:hover {
            background-color: #eaeaea;
        }
        QPushButton:pressed {
            background-color: #dcdcdc;
        }
        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #c8c8c8;
            padding: 4px;
        }
        QTextEdit {
            background-color: #ffffff;
            border: 1px solid #c8c8c8;
        }
    )";

    qApp->setStyleSheet(style);
}

// Hard-coded dark theme
void ThemeManager::applyDarkTheme()
{
    QString style = R"(
        QMainWindow {
            background-color: #121212;
        }
        QWidget {
            color: #e0e0e0;
            background-color: #121212;
            font-family: "Segoe UI";
            font-size: 10pt;
        }
        QMenuBar {
            background-color: #1e1e1e;
            color: #e0e0e0;
        }
        QMenuBar::item:selected {
            background-color: #2a2a2a;
        }
        QMenu {
            background-color: #1e1e1e;
            color: #e0e0e0;
            border: 1px solid #2c2c2c;
        }
        QMenu::item:selected {
            background-color: #333333;
        }
        QPushButton {
            background-color: #2a2a2a;
            border: 1px solid #3a3a3a;
            padding: 6px;
        }
        QPushButton:hover {
            background-color: #333333;
        }
        QPushButton:pressed {
            background-color: #3d3d3d;
        }
        QLineEdit {
            background-color: #1e1e1e;
            border: 1px solid #3a3a3a;
            padding: 4px;
            selection-background-color: #0078d7;
        }
        QTextEdit {
            background-color: #1e1e1e;
            border: 1px solid #3a3a3a;
            selection-background-color: #0078d7;
        }
        QScrollBar:vertical {
            background: #1e1e1e;
            width: 10px;
        }
        QScrollBar::handle:vertical {
            background: #3a3a3a;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #4a4a4a;
        }
    )";

    qApp->setStyleSheet(style);
}
