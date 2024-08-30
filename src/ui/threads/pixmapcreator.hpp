#pragma once

#include "../page.hpp"

#include <QThread>
#include <QLayout>

#include <mupdf/classes.h>

class PixmapCreatorThread : public QThread {
    Q_OBJECT;

public:
    // mupdf::PdfDocument is not thread safe, we can not pass it neither by value or by
    // reference - it causes errors, so we pass path to the file and the thread 
    // instantiates its own copy of mupdf::PdfDocument
    explicit PixmapCreatorThread(std::string path);

signals:
    // pixmap representing page and the number of the corresponding page
    void sigPixmapCreated(QPixmap, int);

private:
    mupdf::PdfDocument m_PdfDoc;

protected:
    void run() override;

};
