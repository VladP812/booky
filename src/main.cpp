#include <QApplication>
#include "ui/documentview.hpp"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    
    PdfView mainWindow("/home/inri/Projects/Programming/AI/booky/pdfs/nlp.pdf");
    mainWindow.show();

    return app.exec();
}
