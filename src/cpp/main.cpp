// DO NOT MOVE THESE INCLUDES - THEY SHOULD ALWAYS STAY ON TOP.
// Otherwise, Qt's MOC interprets slots from Python.h as Qt's slots and fails.
// --------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
// -------------------------------
#include "pythonmodules.hpp"

#include "ui/mainwindow.hpp"

#include <QApplication>

namespace py = pybind11;

void setDarkTheme(QApplication &app) {
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);
}

int main(int argc, char** argv){
    setenv("PYTHONPATH",
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/:"
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/env/lib/"
            "python3.12/site-packages/", 
            1);
    // init the python itself
    py::initialize_interpreter();
    // init and import modules we use in booky
    pymodules::init_modules();
    // releasing the GIL from the main thread so other threads can use it
    py::gil_scoped_release release;


    QApplication app(argc, argv);   
    setDarkTheme(app);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}


