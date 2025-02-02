#include "configurator.hpp"

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QApplication>
#include <QSettings>
#include <QPushButton>

inline void saveConfigValue(const QString& name, const QVariant& value) {
    QSettings settings;
    settings.setValue(name, value);
}

Configurator::Configurator(const std::function<void()>& onLeave, QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* myLayout = new QVBoxLayout(this);
    QLabel* infoLabel = new QLabel("Theme");
    myLayout->addWidget(infoLabel);

    QComboBox* themeSelector = new QComboBox(this);
    themeSelector->addItem("dark");
    themeSelector->addItem("light");

    QSettings settings;
    QString currentTheme = settings.value("color_scheme").toString();

    themeSelector->setCurrentText(currentTheme);

    connect(themeSelector, &QComboBox::currentTextChanged,
            [this](const QString& text) {
            Configurator::setTheme(*qApp, text);
            saveConfigValue("color_scheme", text);
            });

    myLayout->addWidget(themeSelector);
    myLayout->addStretch();

    QPushButton* goBack = new QPushButton("Back", this);
    connect(goBack, &QPushButton::clicked,
            [onLeave](){onLeave();});
    myLayout->addWidget(goBack);
}

void Configurator::setTheme(QApplication& app, const QString& theme) {
    if (theme == "dark") {
        setDarkTheme(app);
    }
    else {
        setLightTheme(app);
    }
}

void Configurator::setLightTheme(QApplication& app) {
    qApp->setPalette(QApplication::style()->standardPalette());
}

void Configurator::setDarkTheme(QApplication& app) {
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);
}

void writeDefaultConfigFirstRun(QSettings& settings) {
    settings.setValue("first_run", false);
    settings.setValue("color_scheme", "light");
}

void Configurator::readConfig(QSettings& settings, QApplication& app) {
    QString colorScheme = settings.value("color_scheme", "").toString();
    Configurator::setTheme(app, colorScheme);
}

void Configurator::initializeConfig(QApplication& app) {
    QCoreApplication::setOrganizationName("booky");
    QCoreApplication::setApplicationName("booky");
    QSettings settings;
    
    bool isFirstRun = settings.value("first_run", true).toBool();
    if (isFirstRun) {
        writeDefaultConfigFirstRun(settings);
    }
    readConfig(settings, app);
}
