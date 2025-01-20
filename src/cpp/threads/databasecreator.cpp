// DO NOT MOVE THESE INCLUDES - THEY SHOULD ALWAYS STAY ON TOP.
// Otherwise, Qt's MOC interprets slots from Python.h as Qt's slots and fails.
// --------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
// -------------------------------

#include "databasecreator.hpp"
#include "tempconsts.hpp"
#include "../appstate.hpp"

#include <stdexcept>
#include <string>
#include <filesystem>

#include <openssl/evp.h>
#include <mupdf/classes.h>
#include <QDebug>

namespace py = pybind11;
namespace fs = std::filesystem;

using namespace mupdf;

DatabaseCreatorThread::DatabaseCreatorThread(std::string path, QObject* parent) 
    : QThread(parent), m_PdfDocument(path) {}

void DatabaseCreatorThread::run(){
    int totalPages = m_PdfDocument.pdf_count_pages();
    
    std::string documentContent = "";
    for(int i = 0; i < totalPages; ++i){
        PdfPage page = m_PdfDocument.pdf_load_page(i);
        std::string pageTextContent = extractTextFromPage(page);
        if(pageTextContent.empty()) continue;
        documentContent += "\n" + pageTextContent;
    }
    
    // TODO: notify a user that no database will be created since the document is empty
    if(documentContent.empty()) return;
    
    std::string documentHash = hashString(documentContent);

    if(!fs::exists(ROOT_DB_PATH) || !fs::is_directory(ROOT_DB_PATH))
        throw std::runtime_error("Could not locate root database directory");
    
    for(const auto& entry: fs::directory_iterator(ROOT_DB_PATH)){
        if(!fs::is_directory(entry)) continue;
        if(entry.path().filename().string() == documentHash){
            std::cout << "An existing database for this document has been found."
                      << std::endl;
            AppState::currentDocumentHash = documentHash;
            return;
        }
    }

    try {
        setenv("PYTHONPATH",
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/:"
            "/home/inri/Projects/Programming/AI/bookycpp/src/python/env/lib/"
            "python3.12/site-packages/", 
            1);
        py::scoped_interpreter guard{};

        py::module langchainDocuments = py::module::import("langchain_core.documents");
        py::object Document = langchainDocuments.attr("Document");

        py::module knowledgebase = py::module::import("knowledgebase.kb");
        py::object generateChromaDb = knowledgebase.attr("generate_and_store_chroma_db");

        std::vector<py::object> pages;

        for(int i = 0; i < totalPages; ++i){
            PdfPage page = m_PdfDocument.pdf_load_page(i);
            std::string strContent = extractTextFromPage(page);
            if(strContent.empty()) continue;
            py::object pyPage = Document(strContent);
            pages.push_back(pyPage);
        }

        std::cout << "Document hash: " << documentHash << std::endl;
        AppState::currentDocumentHash = documentHash;
        generateChromaDb(py::cast(pages), ROOT_DB_PATH + documentHash + "/");
        std::cout << "Vector database created successfully!" << std::endl;
    } catch (const py::error_already_set& err) {
        std::cerr << "DatabaseCreatorThread:: Error running Python" << std::endl;
        PyErr_Print();
    }
}

std::string DatabaseCreatorThread::extractTextFromPage(PdfPage& page) const {
    FzStextOptions opts;
    FzStextPage sTextPage(page.super(), opts);
    std::string textContent = "";
    for(FzStextPageIterator pageIter = sTextPage.begin();
            pageIter!= sTextPage.end(); ++pageIter){
        for(FzStextBlockIterator blckIter = (*pageIter).begin();
                blckIter != (*pageIter).end(); ++blckIter){
            for(FzStextLineIterator lineIter = (*blckIter).begin();
                    lineIter != (*blckIter).end(); ++lineIter){
                textContent += (*lineIter).m_internal->c;
            }
            textContent += " "; // a space instead of \n after each line
        } 
    }
    return sanitizeString(textContent);
}

std::string DatabaseCreatorThread::sanitizeString(const std::string& input) const {
    std::string sanitized;
    std::copy_if(input.begin(), input.end(), std::back_inserter(sanitized), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c))
            || std::isspace(static_cast<unsigned char>(c))
            || std::ispunct(static_cast<unsigned char>(c))
            || c == '\n';
    });
    return sanitized;
}

std::string DatabaseCreatorThread::hashString(const std::string& str) const {
    EVP_MD_CTX* context = EVP_MD_CTX_new();  
    if (context == nullptr) {
        throw std::runtime_error("Failed to create hash context.");
    }

    const EVP_MD* md = EVP_sha256();  
    unsigned char hash[EVP_MAX_MD_SIZE];  
    unsigned int lengthOfHash = 0;

    if (EVP_DigestInit_ex(context, md, nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize the digest.");
    }

    
    if (EVP_DigestUpdate(context, str.c_str(), str.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update the digest.");
    }

    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize the digest.");
    }

    EVP_MD_CTX_free(context);

    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}
