#pragma once

#include "assistant.hpp"
#include "pagescontainer.hpp"
#include "configurator.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>

class MainWindow : public QMainWindow{
public:
    explicit MainWindow();

public slots:
    // trigerred only by sigPagesReady of PagesContainer widget - tells that pages are
    // rendered and ready to be displayed
    void slotDisplayPages();
    void toggleAssitant();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    // to switch between configuration and document view
    QStackedWidget* stackOfWidgets; 
    QGridLayout* documentViewLayout;

    AssistantWidget assistant;
    Configurator configurator;
    QLabel infoTextLabel;
    PagesContainer* pagesContainer;
    QScrollArea* pagesScrollArea;

    QPushButton assistantButton;
    bool isAssistantOpen;

    void repositionOverlay();
    void loadAndDisplayDocument(std::string path);
    void switchToAppearanceSettings();
    void switchToDocumentView();
};
