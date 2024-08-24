#pragma once

#include <QWidget>
#include <mupdf/classes.h>

#include "../pdfselection/sharedtypes.hpp"

class PagesContainer : public QWidget{
    Q_OBJECT;

public:
    explicit PagesContainer(std::string filePath, QWidget* parent = nullptr);

public slots:
    void slotClearSelectionAllPages();
    void slotSetSelectionDirection(SelectionDirection);

signals:
    void sigClearPagesSelection();
    void sigSetSelectionDirection(SelectionDirection);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    SelectionDirection m_SelectionDirection;
};
