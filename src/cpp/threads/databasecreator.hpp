#pragma once

#include <QThread>

#include <memory>
#include <mupdf/classes.h>

class DatabaseCreatorThread : public QThread {
    Q_OBJECT;

public:
    // mupdf::PdfDocument is not thread safe, we can not pass it neither by value or by
    // reference - it causes errors, so we pass path to the file and the thread 
    // instantiates its own copy of mupdf::PdfDocument
   explicit DatabaseCreatorThread(std::string path, QObject* parent = nullptr);

protected:
   void run() override;

private:
   const static std::string m_sRootDbPath;
   mupdf::PdfDocument m_PdfDocument;

   std::string extractTextFromPage(mupdf::PdfPage& page) const;
   std::string sanitizeString(const std::string& input) const;
   std::string hashString(const std::string& str) const;
};
