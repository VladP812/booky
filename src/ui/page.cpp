#include "page.hpp"

#include <QMouseEvent>
#include <mupdf/fitz/geometry.h>

using namespace mupdf;

PdfPageLabel::PdfPageLabel(PdfDocument& doc, int pageNum, QWidget* parent)
    : QLabel(parent),
    m_fitzPage(doc.pdf_load_page(pageNum)),
    m_selection(m_fitzPage, 
        [this](std::vector<fz_rect> rects){m_selectionRects = rects; update();}),
    m_bIsSelectionHolder(false),
    m_bIsSelectionSubject(false)
{}

std::string PdfPageLabel::getSelectedText(){
    if(m_bIsSelectionSubject)
        return m_selection.getSelectedText();
    return "";
}

void PdfPageLabel::slotClearThisSelection(){
    m_selection.clearSelection();
    m_bIsSelectionHolder = m_bIsSelectionSubject = false;
}

void PdfPageLabel::slotSetSelectionDirection(SelectionDirection direction){
    m_selection.setDirection(direction);
}

void PdfPageLabel::mousePressEvent(QMouseEvent* event){
    QWidget::mousePressEvent(event);
    if(event->buttons().testFlag(Qt::RightButton)) return;

    event->accept();

    int x = event->pos().x();
    int y = event->pos().y();

    emit sigClearSelectionAllPages();

    m_selection.clearSelection();
    m_selection.beginSelection(x, y);
    m_bIsSelectionHolder = m_bIsSelectionSubject = true;
}

void PdfPageLabel::mouseMoveEvent(QMouseEvent* event){
    int x = event->pos().x();
    int y = event->pos().y();
    
    if(y >= 0 && y <= size().height()){
        if(!m_bIsSelectionSubject)
            m_bIsSelectionSubject = true;
        m_selection.continueSelection(x, y);
        event->accept();
    }
    else{
        event->ignore();
        if(m_bIsSelectionHolder){
            if(y > size().height())
                emit sigSetSelectionDirectionAllPages(SelectionDirection::DOWN);
            else emit sigSetSelectionDirectionAllPages(SelectionDirection::UP);
        }
    }
}

void PdfPageLabel::mouseReleaseEvent(QMouseEvent* event){
    event->accept();
}

void PdfPageLabel::wheelEvent(QWheelEvent* event){
    QWidget::wheelEvent(event);
    // if we are not selecting text we don't need to do anything 
    if(!event->buttons().testFlag(Qt::LeftButton)) return;
    int x = event->position().x();
    int y = event->position().y();
    
    m_selection.continueSelection(x, y);
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

