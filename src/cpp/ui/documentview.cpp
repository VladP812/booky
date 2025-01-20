#include "documentview.hpp"
#include "assistant.hpp"
#include "pagescontainer.hpp"
#include "../threads/databasecreator.hpp"

#include <QScrollArea>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <mupdf/classes.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
using namespace mupdf;

DocumentView::DocumentView(QWidget* parent)
    : QWidget(parent),
    m_pPagesContainer(nullptr),
    m_pAssistant(new AssistantWidget(this))
{
    QLabel* textLabel = new QLabel("Open a file to begin", this);
    setStyleSheet("border: 1px dashed black;");
}

void DocumentView::loadAndDisplayDocument(std::string path) {
    QLabel* textLabel = new QLabel("Processing your document", this);
    layout()->addWidget(textLabel);

    m_pPagesContainer = new PagesContainer(path, this);

    connect(m_pPagesContainer, &PagesContainer::sigPagesReady,
            this, &DocumentView::slotDisplayPages);

    m_pPagesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pPagesContainer->processDocument();
    DatabaseCreatorThread* dbCreator = new DatabaseCreatorThread(path, this);
    dbCreator->start();
}

void DocumentView::slotDisplayPages(){
    assert(m_pPagesContainer);
    cleanLayout();

    QScrollArea* scrollArea = new QScrollArea(this); 

    scrollArea->setAlignment(Qt::AlignHCenter);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_pPagesContainer);
    layout()->addWidget(scrollArea);
}

void DocumentView::cleanLayout(){
    QLayoutItem* layoutItem;
    while((layoutItem = layout()->takeAt(0)) != nullptr){
        if(layoutItem->widget()) layoutItem->widget()->hide();
        delete layoutItem;
    }
}

QPoint DocumentView::getPagesContainerGlobalPos() {
    if (m_pPagesContainer){
        return m_pPagesContainer->mapToParent(QPoint(0, 0));
    }
    return mapToParent(QPoint(0, 0));
}

QSize DocumentView::getPagesContainerSize() {
    if (m_pPagesContainer) {
        return m_pPagesContainer->size();
    }
    return size();
}
