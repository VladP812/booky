#pragma once

#include "../pdfselection/sharedtypes.hpp"

#include <QWidget>
#include <QThread>
#include <QVBoxLayout>

#include <mupdf/classes.h>

class PagesContainer : public QWidget{
    Q_OBJECT;
public:
    explicit PagesContainer(std::string path, QWidget* parent = nullptr);
    void processDocument();

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
    // adds a single page obtained from DocumentLoaderThread to the layout
    // does not show the layout with pages unless all pages are in the layout,
    // meaning pages will only show in displayPages slot which is connected to
    // the finished() signal of DocumentLoaderThread
    void slotAddPage(QPixmap, int);
    // sets the layout with all pages as its layout so pages become visible
    void slotDisplayPages();

signals:
    // signals to all pages that they need to clear their selection
    // emitted only by slotClearSelectionAllPages
    void sigClearPagesSelection();
    // signals to all pages that they need to set their selection direction
    // emitted only by slotSetSelectionDirection
    void sigSetSelectionDirection(SelectionDirection);
    // signals the document view (parent widget) that all pages are rendered and ready
    // to be displayed, document view adds this pages container widget then. 
    void sigPagesReady();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    mupdf::PdfDocument m_fitzPdfDocument;
    std::string m_sFilePath;
};
