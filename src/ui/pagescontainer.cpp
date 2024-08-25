#include "pagescontainer.hpp"
#include "page.hpp"
#include "contextmenu.hpp"

#include <mupdf/classes.h>

#include <QVBoxLayout>
#include <QClipboard>

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
        // signal from page where selection started to set the direction of selection
        //  - either up or down
        connect(pdfPage, &PdfPageLabel::sigSetSelectionDirectionAllPages,
                this, &PagesContainer::slotSetSelectionDirectionAllPages);
        // signal from container to all pages to set their selection directions
        connect(this, &PagesContainer::sigSetSelectionDirection,
                pdfPage, &PdfPageLabel::slotSetSelectionDirection);
        pdfContainerLayout->addWidget(pdfPage);
    }
}

void PagesContainer::slotClearSelectionAllPages(){
    emit sigClearPagesSelection();
}

void PagesContainer::slotSetSelectionDirectionAllPages(SelectionDirection dir){
    emit sigSetSelectionDirection(dir);
}
void PagesContainer::slotCopySelectedTextAllPages(){
    std::string resStr =  "";
    int numPages = layout()->count();
    for(int i = 0; i < numPages; ++i){
        qDebug() << "here 1 " + std::to_string(i);
        QLayoutItem* item = layout()->itemAt(i);
        PdfPageLabel* pageLabel = static_cast<PdfPageLabel*>(item->widget());
        if(!pageLabel)
            continue;
        std::string copiedText = pageLabel->getSelectedText();
        if(copiedText == "")
            continue;
        resStr += copiedText;
        resStr += "\n";
    }
    QClipboard* clipboard = QGuiApplication ::clipboard();
    clipboard->setText(QString(resStr.c_str()));
}

void PagesContainer::mousePressEvent(QMouseEvent* event){
    if(event->buttons().testFlag(Qt::RightButton)){
        ContextMenu contextMenu(this);
        QAction copyTextAction("Copy Selected Text", this);
        connect(&copyTextAction, &QAction::triggered,
                this, &PagesContainer::slotCopySelectedTextAllPages);
        contextMenu.addAction(&copyTextAction);
        contextMenu.exec(event->globalPosition().toPoint());
        return;
    }

    qDebug() << "container - press";
    event->accept();
}

void PagesContainer::mouseMoveEvent(QMouseEvent* event){
    qDebug() << "container - move";
    event->accept();
    QPoint pos = event->pos();
    qDebug() << pos.x();
    qDebug() << pos.y();
    QPoint resolvedPos = mapFromGlobal(event->globalPosition().toPoint()); 
    QWidget* widget = childAt(resolvedPos);

    if(!widget) return;

    QMouseEvent* newMouseEvent = new QMouseEvent(event->type(), 
                        widget->mapFromGlobal(event->globalPosition()),
                        event->globalPosition(), event->button(), event->buttons(),
                        event->modifiers());
    QCoreApplication::postEvent(widget, newMouseEvent);
}

void PagesContainer::mouseReleaseEvent(QMouseEvent* event){
    qDebug() << "container - release";
    event->accept();
}


