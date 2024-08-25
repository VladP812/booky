#pragma once

#include "../pdfselection/pageselection.hpp"
#include "../pdfselection/sharedtypes.hpp"

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <mupdf/classes.h>

class PdfPageLabel : public QLabel {

    Q_OBJECT;

public:
    explicit PdfPageLabel(mupdf::PdfDocument doc, int pageNum, 
                          QWidget* parent=nullptr);
    std::string getSelectedText();

public slots:
    // clears selection on this page
    void slotClearThisSelection();
    // sets the direction of the selection on this page
    void slotSetSelectionDirection(SelectionDirection);

signals:
    // signals that selection should be cleared for all pages
    // it is being emitted on mousePressEvent if left mouse button is pressed
    void sigClearSelectionAllPages();
    // signals that selection direction should be set for all pages.
    // it is being emitted if cursor goes outside of this page's bounding box
    // while selecting text. either up or down.
    void sigSetSelectionDirectionAllPages(SelectionDirection);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    mupdf::PdfPage m_fitzPage;
    PageSelection m_selection;
    // set to true only on mousePress event - meaning the only page which has this flag
    // set to true is the one which received mousePressEvent
    bool m_bIsSelectionHolder;
    // set to true when page has selected text, false when it does not.
    bool m_bIsSelectionSubject;
    // holds bounding boxes of selected characters as rectangles
    // it is being set by the PageSelection via the callback
    // and then the page renders them by calling update which calls paintEvent
    std::vector<fz_rect> m_selectionRects;
};
