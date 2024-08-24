#pragma once

#include "sharedtypes.hpp"

#include <functional>
#include <mupdf/classes.h>

using CBPageRenderSelection = std::function<void(std::vector<fz_rect>)>;

class PageSelection{
public:
   explicit PageSelection(mupdf::PdfPage fzPdfPage,
                          CBPageRenderSelection cbRenderSelection);

    void beginSelection(int x, int y);
    void continueSelection(int x, int y);
    void clearSelection();
    void selectAll();
    void setDirection(SelectionDirection direction);

private:
    static mupdf::FzStextOptions m_sFzStextOptions;
    const CBPageRenderSelection callbackRenderSelection;
    mupdf::FzStextPage m_fitzPage;
    mupdf::FzPoint m_selectionBegin;
    mupdf::FzPoint m_selectionEnd;

    void processSelection() const;
};
