#pragma once

#include <QWidget>
#include <mupdf/classes.h>

#include "../pdfselection/sharedtypes.hpp"

class PagesContainer : public QWidget{
    Q_OBJECT;

public:
    explicit PagesContainer(std::string filePath, QWidget* parent = nullptr);

public slots:
    // clears selection of all pages by emitting the sigClearPagesSelection signal
    // triggered only by page's (not container's) sigClearSelectionAllPages
    void slotClearSelectionAllPages();
    // sets direction of selection for all pages, either up or down
    // triggered only by page's (not container's) sigSetSelectionDirectionAllPages
    void slotSetSelectionDirectionAllPages(SelectionDirection);
    // copies text from all pages with selected text into the system clipboard
    // triggered only by context menu action
    void slotCopySelectedTextAllPages();

signals:
    // signals to all pages that they need to clear their selection
    // emitted only by slotClearSelectionAllPages
    void sigClearPagesSelection();
    // signals to all pages that they need to set their selection direction
    // emitted only by slotSetSelectionDirection
    void sigSetSelectionDirection(SelectionDirection);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

};
