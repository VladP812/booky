#include "documentview.hpp"
#include "pagescontainer.hpp"

#include <QScrollArea>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>

#include <mupdf/classes.h>

using namespace mupdf;

DocumentView::DocumentView(QWidget* parent)
    : QWidget(parent),
    m_pPagesContainer(nullptr)
{
    QVBoxLayout* vboxLayout = new QVBoxLayout(this);
    vboxLayout->setAlignment(Qt::AlignHCenter);
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* textLabel = new QLabel("Open a file to begin", this);
    vboxLayout->addWidget(textLabel);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void DocumentView::loadAndDisplayDocument(std::string path) {
    cleanLayout();
    QLabel* textLabel = new QLabel("Processing your document", this);
    layout()->addWidget(textLabel);

    m_pPagesContainer = new PagesContainer(path, this);
    connect(m_pPagesContainer, &PagesContainer::sigPagesReady,
            this, &DocumentView::slotDisplayPages);
    m_pPagesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pPagesContainer->processDocument();
}

void DocumentView::slotDisplayPages(){
    if(!m_pPagesContainer) return;
    QScrollArea* scrollArea = new QScrollArea(this); 
    cleanLayout();
    layout()->addWidget(scrollArea);
    scrollArea->setAlignment(Qt::AlignHCenter);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_pPagesContainer);
}

void DocumentView::cleanLayout(){
    QLayoutItem* layoutItem;
    while((layoutItem = layout()->takeAt(0)) != nullptr){
        if(layoutItem->widget()) layoutItem->widget()->hide();
        delete layoutItem;
    }
}
