#pragma once

#include <QWidget>
#include <mupdf/classes.h>
#include <qtmetamacros.h>

class PagesContainer : public QWidget{
    Q_OBJECT;

public:
    explicit PagesContainer(std::string filePath, QWidget* parent = nullptr);

public slots:
    void slotClearSelectionAllPages();

signals:
    void sigClearPagesSelection();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
