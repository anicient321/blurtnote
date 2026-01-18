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

    void applyTheme(Theme theme);          // apply theme and save in Qsettings
    Theme currentTheme() const;            // return current active theme
    void loadThemeOnStartup();             // load theme from Qsettings
    void saveCurrentTheme();               // save current active theme

private:
    explicit ThemeManager(QObject *parent = nullptr);

    void applyLightTheme();                // hard-coded light theme
    void applyDarkTheme();                 // hard-coded dark theme

    Theme m_currentTheme;
};

#endif // THEMEMANAGER_H
