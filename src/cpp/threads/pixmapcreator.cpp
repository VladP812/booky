#include "pixmapcreator.hpp"

#include <QImage>
#include <QPixmap>

using namespace mupdf;

PixmapCreatorThread::PixmapCreatorThread(std::string path, QObject* parent) 
    : QThread(parent),
    m_PdfDoc(path) {}

void PixmapCreatorThread::run(){
    int pagesTotal = m_PdfDoc.pdf_count_pages();
    for(int i = 0; i < pagesTotal; ++i){
        PdfPage page = m_PdfDoc.pdf_load_page(i);
        FzMatrix transformMatrix;
        transformMatrix = transformMatrix.fz_scale(1, 1);
        FzPixmap fzPagePixmap = page.pdf_new_pixmap_from_page_with_usage(
                                        transformMatrix, FzColorspace::Fixed_RGB, 
                                        0, "View", FZ_MEDIA_BOX);
        QImage qImg(fzPagePixmap.samples(), fzPagePixmap.w(), fzPagePixmap.h(),
                    fzPagePixmap.stride(), QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(qImg);
        emit sigPixmapCreated(pixmap, i);
    }
}
