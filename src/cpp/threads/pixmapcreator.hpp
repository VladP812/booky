#pragma once

#include <QThread>
#include <QLayout>
#include <QPixmap>

#include <mupdf/classes.h>

/*
 * This thread is used to create pixmaps of pages of a document.
 * Since it takes quite a lot of time, it is better to do in a separate thread
 * so the ui does not freeze.
*/

class PixmapCreatorThread : public QThread {
    Q_OBJECT;

public:
    // mupdf::PdfDocument is not thread safe, we can not pass it neither by value or by
    // reference - it causes errors, so we pass path to the file and the thread 
    // instantiates its own copy of mupdf::PdfDocument
    explicit PixmapCreatorThread(std::string path, QObject* parent = nullptr);

signals:
    // pixmap representing page and the number of the corresponding page,
    // emitted only to the PagesContainer
    void sigPixmapCreated(QPixmap, int);

protected:
    void run() override;

private:
    mupdf::PdfDocument m_PdfDoc;

};
