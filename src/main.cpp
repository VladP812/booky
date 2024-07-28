#include <QApplication>
#include "ui/pdfview.hpp"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    
    PdfView mainWindow("/home/inri/Projects/Programming/AI/booky/pdfs/nlp.pdf");
    mainWindow.show();

    return app.exec();
}
