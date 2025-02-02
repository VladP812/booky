// DO NOT MOVE THESE INCLUDES - THEY SHOULD ALWAYS STAY ON TOP.
// Otherwise, Qt's MOC interprets slots from Python.h as Qt's slots and fails.
// --------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
// -------------------------------
#include "pythonmodules.hpp"

#include "ui/widgets/mainwindow.hpp"
#include "ui/widgets/configurator.hpp"

#include <QApplication>
#include <QSettings>

namespace py = pybind11;


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
    Configurator::initializeConfig(app);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}


