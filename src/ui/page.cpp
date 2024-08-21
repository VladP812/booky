#include "page.hpp"

#include <QMouseEvent>
#include <mupdf/fitz/geometry.h>
#include <string>

using namespace mupdf;

PdfPageLabel::PdfPageLabel(PdfDocument doc, int pageNum, QWidget* parent)
    : QLabel(parent),
    m_fitzPage(doc.pdf_load_page(pageNum)),
    m_selection(m_fitzPage, 
        [this](std::vector<fz_rect> rects){m_selectionRects = rects; update();}
    )
{
    PdfPage page = doc.pdf_load_page(pageNum);
    FzMatrix transformMatrix;
    transformMatrix = transformMatrix.fz_scale(1, 1);

    FzPixmap fzPagePixmap = page.pdf_new_pixmap_from_page_with_usage(
                                    transformMatrix, FzColorspace::Fixed_RGB, 
                                    0, "View", FZ_MEDIA_BOX);
    QImage qImg(fzPagePixmap.samples(), fzPagePixmap.w(), fzPagePixmap.h(),
                fzPagePixmap.stride(), QImage::Format_RGB888);
    setFixedSize(qImg.width(), qImg.height());
    setPixmap(QPixmap::fromImage(qImg));
}

void PdfPageLabel::slotClearThisSelection() {
    m_selection.clearSelection();
}

void PdfPageLabel::mousePressEvent(QMouseEvent* event){
    qDebug() << "page " + std::to_string(m_fitzPage.m_internal->super.number) + " - press";
    event->accept();
    int x = event->pos().x();
    int y = event->pos().y();
    emit sigClearSelectionAllPages();
    m_selection.clearSelection();
    m_selection.beginSelection(x, y);
}

void PdfPageLabel::mouseMoveEvent(QMouseEvent* event){
    qDebug() << "page " + std::to_string(m_fitzPage.m_internal->super.number) + " - move";
    int x = event->pos().x();
    int y = event->pos().y();
    
    qDebug() << "x = " + std::to_string(x);
    qDebug() << "y = " + std::to_string(y);

    if(y >= 0 && y <= size().height()){
        m_selection.continueSelection(x, y);
        event->accept();
        qDebug() << "accept";
    }
    else{
        event->ignore();
        qDebug() << "ignore";
    }
}

void PdfPageLabel::mouseReleaseEvent(QMouseEvent* event){
    qDebug() << "page " + std::to_string(m_fitzPage.m_internal->super.number) + " - release";
    event->accept();
}

void PdfPageLabel::paintEvent(QPaintEvent* event){
    QLabel::paintEvent(event);

    if(m_selectionRects.empty()) return;

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 255, 42)));
    for(const auto& rect : m_selectionRects){
        QRect qrect(rect.x0, rect.y0, rect.x1 - rect.x0, rect.y1 - rect.y0);
        painter.drawRect(qrect);
    }
}

