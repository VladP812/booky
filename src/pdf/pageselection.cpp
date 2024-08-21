#include "pageselection.hpp"
#include <mupdf/extra.h>
#include <mupdf/fitz/geometry.h>

using namespace mupdf;

FzStextOptions PageSelection::m_sFzStextOptions = FzStextOptions();

PageSelection::PageSelection(PdfPage fzPdfPage, CBPageRenderSelection cbRenderSelection)
    : m_fitzPage(fzPdfPage.super(), m_sFzStextOptions),
    callbackRenderSelection(cbRenderSelection),
    m_selectionTopLeft(0, 0),
    m_selectionBottomRight(0, 0)
{}

void PageSelection::beginSelection(int x, int y){
    m_selectionTopLeft.x = m_selectionBottomRight.x = x;
    m_selectionTopLeft.y = m_selectionBottomRight.y = y;
}

void PageSelection::continueSelection(int x, int y){
    m_selectionBottomRight.x = x;
    m_selectionBottomRight.y = y;
    processSelection();
}

void PageSelection::processSelection() const {
    std::vector<fz_quad> quadsToHighlight = m_fitzPage.fz_highlight_selection2(
                                                m_selectionTopLeft,
                                                m_selectionBottomRight,
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
    m_selectionTopLeft.x = m_selectionTopLeft.y = 
        m_selectionBottomRight.x = m_selectionBottomRight.y = 0;
   callbackRenderSelection(std::vector<fz_rect>());
}

void PageSelection::selectAll() {
    m_selectionTopLeft.x = m_selectionTopLeft.y = 0;
    m_selectionBottomRight.x = m_fitzPage.m_internal->mediabox.x1;
    m_selectionBottomRight.y = m_fitzPage.m_internal->mediabox.y1;

    processSelection();
}
