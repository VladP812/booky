#include "pdfpage.hpp"

using namespace mupdf;

PdfPageLabel::PdfPageLabel(PdfDocument doc, int pageNum, QWidget* parent)
    : QLabel(parent)
{
    PdfPage page = doc.pdf_load_page(pageNum);
    FzMatrix transformMatrix;
    transformMatrix = transformMatrix.fz_scale(1, 1);

    FzPixmap fzPagePixmap = page.pdf_new_pixmap_from_page_with_usage(
                                    transformMatrix, FzColorspace::Fixed_RGB, 
                                    0, "View", FZ_MEDIA_BOX);
    QImage qImg(fzPagePixmap.samples(), fzPagePixmap.w(), fzPagePixmap.h(),
                fzPagePixmap.stride(), QImage::Format_RGB888);
    setFixedSize(qImg.width(), qImg.height());
    setPixmap(QPixmap::fromImage(qImg));
}
