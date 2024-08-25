#include "pageselection.hpp"

#include <qdebug.h>

using namespace mupdf;

FzStextOptions PageSelection::m_sFzStextOptions = FzStextOptions();

PageSelection::PageSelection(PdfPage fzPdfPage,
                             CBPageRenderSelection cbRenderSelection)
    : m_fitzPage(fzPdfPage.super(), m_sFzStextOptions),
    callbackRenderSelection(cbRenderSelection),
    m_selectionBegin(0, 0),
    m_selectionEnd(0, 0)
{}

std::string PageSelection::getSelectedText() {
    qDebug() << "Selection begin: x=" << std::to_string(m_selectionBegin.x)
        << " y=" + std::to_string(m_selectionBegin.y);
    qDebug() << "Selection end: x=" << std::to_string(m_selectionEnd.x)
        << " y=" + std::to_string(m_selectionEnd.y);
    return m_fitzPage.fz_copy_selection(m_selectionBegin, m_selectionEnd, 0);
}

void PageSelection::beginSelection(int x, int y){
    m_selectionBegin.x = m_selectionEnd.x = x;
    m_selectionBegin.y = m_selectionEnd.y = y;
}

void PageSelection::continueSelection(int x, int y){
    m_selectionEnd.x = x;
    m_selectionEnd.y = y;
    processSelection();
}

void PageSelection::setDirection(SelectionDirection direction){
    if(m_selectionBegin.x == 0 && m_selectionBegin.y == 0){
        if(direction == SelectionDirection::UP){
            m_selectionBegin.x = m_fitzPage.m_internal->mediabox.x1;
            m_selectionBegin.y = m_fitzPage.m_internal->mediabox.y1;
        }
        else {
            m_selectionBegin.x = 0;
            m_selectionBegin.y = 0;
        }
    }
}

void PageSelection::processSelection() const {
    std::vector<fz_quad> quadsToHighlight = m_fitzPage.fz_highlight_selection2(
                                                m_selectionBegin,
                                                m_selectionEnd,
                                                1024); // truly magic number (max quads)
    if(quadsToHighlight.empty())
        return;

    std::vector<fz_rect> rectsToHighlight;
    for(const auto& quad : quadsToHighlight){
        rectsToHighlight.push_back(fz_rect_from_quad(quad));
    }

    callbackRenderSelection(rectsToHighlight);
}

void PageSelection::clearSelection() {
    m_selectionBegin.x = m_selectionBegin.y = 
        m_selectionEnd.x = m_selectionEnd.y = 0;
   callbackRenderSelection(std::vector<fz_rect>());
}

void PageSelection::selectAll() {
    m_selectionBegin.x = m_selectionBegin.y = 0;
    m_selectionEnd.x = m_fitzPage.m_internal->mediabox.x1;
    m_selectionEnd.y = m_fitzPage.m_internal->mediabox.y1;

    processSelection();
}
