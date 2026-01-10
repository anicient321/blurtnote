#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum class Theme {
        Light,
        Dark
    };
    Q_ENUM(Theme)

    static ThemeManager& instance();

    void applyTheme(Theme theme);          // primijeni temu i sprema u QSettings
    Theme currentTheme() const;            // vrati trenutno aktivnu temu
    void loadThemeOnStartup();             // učitaj temu iz QSettings
    void saveCurrentTheme();               // spremi trenutno aktivnu temu

private:
    explicit ThemeManager(QObject *parent = nullptr);

    void applyLightTheme();                // hard-coded light theme
    void applyDarkTheme();                 // hard-coded dark theme

    Theme m_currentTheme;
};

#endif // THEMEMANAGER_H
