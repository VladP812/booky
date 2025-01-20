#pragma once

#include "pagescontainer.hpp"
#include "assistant.hpp"

#include <QMainWindow>
#include <QThread>
#include <QPushButton>

class DocumentView: public QWidget {
    Q_OBJECT;
public:
    explicit DocumentView(QWidget* parent);
    void loadAndDisplayDocument(std::string path);
    QPoint getPagesContainerGlobalPos();    
    QSize getPagesContainerSize();

public slots:
    // trigerred only by sigPagesReady of PagesContainer widget - tells that pages are
    // rendered and ready to be displayed
    void slotDisplayPages();

private:
    PagesContainer* m_pPagesContainer;
    AssistantWidget* m_pAssistant;
    QPushButton* m_pToggleAssistantButton;

    void cleanLayout();
};

