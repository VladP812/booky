#include "pdfview.hpp"
#include "pdfpage.hpp"

#include <QScrollArea>
#include <mupdf/classes.h>

using namespace mupdf;

PdfView::PdfView(std::string filePath, QWidget* parent)
    : QMainWindow(parent),
    m_mainWidget(this)
{
    QVBoxLayout* mainWidgetLayout = new QVBoxLayout(&m_mainWidget);

    QWidget* pdfContainer = new QWidget(&m_mainWidget);
    QVBoxLayout* pdfContainerLayout = new QVBoxLayout(pdfContainer);
    pdfContainerLayout->setAlignment(Qt::AlignCenter);

    PdfDocument doc(filePath);
    int pagesTotal = doc.pdf_count_pages();

    for(int i = 0; i < pagesTotal; ++i){
        PdfPageLabel* pdfPage = new PdfPageLabel(doc, i, pdfContainer);
        pdfContainerLayout->addWidget(pdfPage);
    }

    QScrollArea* scrollArea = new QScrollArea(&m_mainWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(pdfContainer);

    mainWidgetLayout->addWidget(scrollArea);

    setCentralWidget(&m_mainWidget);
}
