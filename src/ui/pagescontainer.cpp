#include "pagescontainer.hpp"
#include "page.hpp"

#include <mupdf/classes.h>

#include <QVBoxLayout>

using namespace mupdf;

PagesContainer::PagesContainer(std::string filePath, QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* pdfContainerLayout = new QVBoxLayout(this);
    pdfContainerLayout->setAlignment(Qt::AlignCenter);

    PdfDocument doc(filePath);
    int pagesTotal = doc.pdf_count_pages();

    for(int i = 0; i < pagesTotal; ++i){
        PdfPageLabel* pdfPage = new PdfPageLabel(doc, i, this);
        
        // signal from any page to the container to clear selection of all pages
        connect(pdfPage, &PdfPageLabel::sigClearSelectionAllPages,
                this, &PagesContainer::slotClearSelectionAllPages);
        // signal from the container to all pages to clear their selections
        connect(this, &PagesContainer::sigClearPagesSelection,
                pdfPage, &PdfPageLabel::slotClearThisSelection);

        pdfContainerLayout->addWidget(pdfPage);
    }
}

void PagesContainer::slotClearSelectionAllPages() {
    emit sigClearPagesSelection();
}

void PagesContainer::mousePressEvent(QMouseEvent* event){
    qDebug() << "container - press";
    event->accept();
}

void PagesContainer::mouseMoveEvent(QMouseEvent* event){
    qDebug() << "container - move";
    event->accept();
    
    QPointF resolvedPosF = mapFromGlobal(event->globalPosition()); 
    QPoint resolvedPos(static_cast<int>(resolvedPosF.x()), static_cast<int>(resolvedPosF.y()));
    QWidget* widget = childAt(resolvedPos);

    if(!widget) return;

    PdfPageLabel* pageLabel = static_cast<PdfPageLabel*>(widget);
    QMouseEvent* newMouseEvent = new QMouseEvent(event->type(), 
                        pageLabel->mapFromGlobal(event->globalPosition()),
                        event->globalPosition(), event->button(), event->buttons(),
                        event->modifiers());
    QCoreApplication::postEvent(pageLabel, newMouseEvent);
}

void PagesContainer::mouseReleaseEvent(QMouseEvent* event){
    qDebug() << "container - release";
    event->accept();
}
