#pragma once

#include <QWidget>
#include <QSettings>

class Configurator : public QWidget {
public:
    Configurator(const std::function<void()>& onLeave, QWidget* parent = nullptr);

    static void initializeConfig(QApplication& app);

private:
    static void setTheme(QApplication& app, const QString& theme);
    static void setDarkTheme(QApplication& app);
    static void setLightTheme(QApplication& app);
    static void readConfig(QSettings& settings, QApplication& app);
};
