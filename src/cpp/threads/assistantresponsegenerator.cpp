// MUST ALWAYS STAY ON TOP
#include "../pythonmodules.hpp"

#include "assistantresponsegenerator.hpp"
#include "tempconsts.hpp"
#include "../appstate.hpp"

#include <iostream>
#include <vector>


namespace py = pybind11;

AssistantResponseGenerator::AssistantResponseGenerator(QString query, QObject* parent)
    : QThread(parent),
    query(query)
{}

void AssistantResponseGenerator::run() {
    std::cout << "here1" << std::endl;
    try {
        py::gil_scoped_acquire acquire;

        py::str res = pymodules::askAssistant(query.toStdString(), false);
                                    //ROOT_DB_PATH + AppState::currentDocumentHash + "/");
        emit responseGenerated(QString::fromStdString(res.cast<std::string>()));
    }
    catch (const py::error_already_set& err) {
        std::cerr << "AssistantResponseGenerator:: Error running Python" << std::endl;
        PyErr_Print();
    }
}


