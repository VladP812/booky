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
    std::string getSelectedText() const;

public slots:
    void slotClearThisSelection();
    void slotSetSelectionDirection(SelectionDirection);

signals:
    void sigClearSelectionAllPages();
    void sigSetSelectionDirection(SelectionDirection);


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    mupdf::PdfPage m_fitzPage;
    PageSelection m_selection;
    bool m_bIsSelectionHolder;
    
    std::vector<fz_rect> m_selectionRects;
};
