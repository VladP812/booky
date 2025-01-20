#include "pagescontainer.hpp"
#include "page.hpp"
#include "contextmenu.hpp"
#include "../threads/pixmapcreator.hpp"

#include <mupdf/classes.h>

#include <QVBoxLayout>
#include <QClipboard>
#include <QVBoxLayout>

using namespace mupdf;

PagesContainer::PagesContainer(std::string filePath, QWidget* parent)
    : QWidget(parent),
    m_fitzPdfDocument(filePath),
    m_sFilePath(filePath)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
}

void PagesContainer::processDocument(){
    layout()->setAlignment(Qt::AlignCenter);
    PixmapCreatorThread* pixmapCreatorThread = new PixmapCreatorThread(m_sFilePath, this);
    connect(pixmapCreatorThread, &PixmapCreatorThread::sigPixmapCreated,
            this, &PagesContainer::slotAddPage);
    connect(pixmapCreatorThread, &PixmapCreatorThread::finished,
            this, &PagesContainer::slotDisplayPages);
    connect(pixmapCreatorThread, &PixmapCreatorThread::finished,
            pixmapCreatorThread, &QObject::deleteLater);
    pixmapCreatorThread->start();
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

void PagesContainer::slotAddPage(QPixmap pixmap, int pageNumber){
    PdfPageLabel* page = new PdfPageLabel(m_fitzPdfDocument, pageNumber, this);
    // signal from any page to the container to clear selection of all pages
    connect(page, &PdfPageLabel::sigClearSelectionAllPages,
            this, &PagesContainer::slotClearSelectionAllPages);
    // signal from the container to all pages to clear their selections
    connect(this, &PagesContainer::sigClearPagesSelection,
            page, &PdfPageLabel::slotClearThisSelection);
    // signal from page where selection started to set the direction of selection
    //  - either up or down
    connect(page, &PdfPageLabel::sigSetSelectionDirectionAllPages,
            this, &PagesContainer::slotSetSelectionDirectionAllPages);
    // signal from container to all pages to set their selection directions
    connect(this, &PagesContainer::sigSetSelectionDirection,
            page, &PdfPageLabel::slotSetSelectionDirection);
    page->setPixmap(pixmap);
    page->setFixedSize(pixmap.size());
    layout()->addWidget(page);   
}

void PagesContainer::slotDisplayPages(){
    emit sigPagesReady();
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

    event->accept();
}

void PagesContainer::mouseMoveEvent(QMouseEvent* event){
    event->accept();
    QPoint pos = event->pos();
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
    event->accept();
}
