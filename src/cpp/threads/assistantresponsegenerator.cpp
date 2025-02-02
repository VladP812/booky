// MUST ALWAYS STAY ON TOP
#include "../pythonmodules.hpp"

#include "assistantresponsegenerator.hpp"
#include "../appstate.hpp"
#include "tempconsts.hpp"

#include <iostream>

namespace py = pybind11;

AssistantResponseGenerator::AssistantResponseGenerator(QString query, bool useKnowledgebase,
                                                       QObject* parent)
    : QThread(parent),
    query(query),
    useKnowledgebase(useKnowledgebase)
{}

void AssistantResponseGenerator::run() {
    try {
        py::gil_scoped_acquire acquire;
        if(useKnowledgebase) {
            py::str res = pymodules::askAssistant(query.toStdString(), true,
                                        ROOT_DB_PATH + AppState::currentDocumentHash + "/");
            emit responseGenerated(QString::fromStdString(res.cast<std::string>()));
        }
        else {
            py::str res = pymodules::askAssistant(query.toStdString(), false);
            emit responseGenerated(QString::fromStdString(res.cast<std::string>()));
        }
    }
    catch (const py::error_already_set& err) {
        std::cerr << "AssistantResponseGenerator:: Error running Python" << std::endl;
        PyErr_Print();
    }
}


