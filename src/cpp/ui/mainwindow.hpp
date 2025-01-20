#pragma once

#include "assistant.hpp"
#include "pagescontainer.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>

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
    QGridLayout* rootLayout;

    QPushButton assistantButton;
    bool isAssistantOpen;
    AssistantWidget assistant;

    QLabel infoTextLabel;
    PagesContainer* pagesContainer;
    QScrollArea* pagesScrollArea;

    void repositionOverlay();
    void loadAndDisplayDocument(std::string path);
};
