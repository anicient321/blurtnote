#include "themeManager.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QDebug>

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

static QString loadStyleFromResource(const QString &resourcePath)
{
    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "blurtnote: - theme resource not found:" << resourcePath;
        return QString();
    }
    const QString content = QString::fromUtf8(f.readAll());
    qDebug() << "blurtnote: + loaded style from" << resourcePath << ":" << content.size() << "bytes";
    return content;
}

// Light theme - load from resource with fallback
void ThemeManager::applyLightTheme()
{
    QString style = loadStyleFromResource(":/themes/light.qss");
    if (style.isEmpty()) {
        qWarning() << "blurtnote: + fell back to inline light style";
        style = R"(
            QMainWindow { background-color: #ffffff; }
            QWidget { color: #000000; background-color: #ffffff; }
        )";
    }
    qApp->setStyleSheet(style);
    qDebug() << "blurtnote: + applied light theme (size:" << style.size() << ")";
}

// Dark theme - load from resource with fallback
void ThemeManager::applyDarkTheme()
{
    QString style = loadStyleFromResource(":/themes/dark.qss");
    if (style.isEmpty()) {
        qWarning() << "blurtnote: + fell back to inline dark style";
        style = R"(
            QMainWindow { background-color: #121212; }
            QWidget { color: #e0e0e0; background-color: #121212; }
        )";
    }
    qApp->setStyleSheet(style);
    qDebug() << "blurtnote: + applied dark theme (size:" << style.size() << ")";
}
