#pragma once

#include "../pdf/pageselection.hpp"

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

signals:
    void sigClearSelectionAllPages();


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;


private:
    mupdf::PdfPage m_fitzPage;
    PageSelection m_selection;
    
    std::vector<fz_rect> m_selectionRects;
};
