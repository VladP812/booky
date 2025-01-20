#pragma once

#include <pybind11/pybind11.h>

namespace pymodules {
    namespace py = pybind11;

    inline py::module langchainDocuments;
    inline py::object Document;

    inline py::module knowledgebase;
    inline py::object generateChromaDb;
    inline py::object askAssistant;

    inline void init_modules() {
        py::module langchainDocuments = py::module::import("langchain_core.documents");
        py::object Document = langchainDocuments.attr("Document");

        knowledgebase = py::module::import("knowledgebase.kb");
        generateChromaDb = knowledgebase.attr("generate_and_store_chroma_db");
        askAssistant = knowledgebase.attr("ask_assistant");
    }
}
