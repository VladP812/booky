#pragma once

#include <QLabel>
#include <mupdf/classes.h>

class PdfPageLabel : public QLabel {
    Q_OBJECT;

public:
    explicit PdfPageLabel(mupdf::PdfDocument doc, int pageNum,
                          QWidget* parent=nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
