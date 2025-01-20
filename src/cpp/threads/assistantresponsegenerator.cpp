// DO NOT MOVE THESE INCLUDES - THEY SHOULD ALWAYS STAY ON TOP.
// Otherwise, Qt's MOC interprets slots from Python.h as Qt's slots and fails.
// --------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
// -------------------------------

#include <iostream>
#include <vector>

#include "assistantresponsegenerator.hpp"
#include "tempconsts.hpp"
#include "../appstate.hpp"

namespace py = pybind11;

AssistantResponseGenerator::AssistantResponseGenerator(QString query, QObject* parent)
    : QThread(parent),
    query(query)
{}

void AssistantResponseGenerator::run() {
    std::cout << "here1" << std::endl;
    try {
        setenv("PYTHONPATH",
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/:"
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/env/lib/"
            "python3.12/site-packages/", 
            1);
        py::scoped_interpreter guard{};
        std::cout << "here2" << std::endl;

        py::module knowledgebase = py::module::import("knowledgebase.kb");
        py::object queryChromaDb = knowledgebase.attr("query_db");

        std::cout << "here3" << std::endl;

        py::list res = queryChromaDb(query.toStdString(),
                                    ROOT_DB_PATH + AppState::currentDocumentHash + "/");
        std::vector<std::pair<std::string, float>> results;
        
        for (auto item : res) {
            auto docAndScore = item.cast<py::tuple>();
            auto pageContent = docAndScore[0].attr("page_content").cast<std::string>();
            auto score = docAndScore[1].cast<float>();
            std::cout << pageContent << std::endl << score << std::endl;
        }
        emit responseGenerated(QString("Assistant Response"));
    }
    catch (const py::error_already_set& err) {
        std::cerr << "AssistantResponseGenerator:: Error running Python" << std::endl;
        PyErr_Print();
    }
}


