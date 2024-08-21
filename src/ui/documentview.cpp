#include "documentview.hpp"
#include "page.hpp"
#include "pagescontainer.hpp"

#include <QScrollArea>
#include <QMouseEvent>
#include <mupdf/classes.h>

using namespace mupdf;

PdfView::PdfView(std::string filePath, QWidget* parent)
    : QMainWindow(parent),
    m_mainWidget(this)
{
    QVBoxLayout* mainWidgetLayout = new QVBoxLayout(&m_mainWidget);

    PagesContainer* pdfContainer = new PagesContainer(filePath, &m_mainWidget);

    QScrollArea* scrollArea = new QScrollArea(&m_mainWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(pdfContainer);

    mainWidgetLayout->addWidget(scrollArea);

    setCentralWidget(&m_mainWidget);
}

void PdfView::mousePressEvent(QMouseEvent* event){
}

void PdfView::mouseMoveEvent(QMouseEvent* event){
    std::cout << "View - Move" << std::endl;
    event->ignore();
}

void PdfView::mouseReleaseEvent(QMouseEvent* event){}
