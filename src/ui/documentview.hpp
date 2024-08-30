#pragma once

#include "pagescontainer.hpp"

#include <QMainWindow>
#include <QThread>

class DocumentView: public QWidget {
    Q_OBJECT;

public:
    explicit DocumentView(QWidget* parent);
    void loadAndDisplayDocument(std::string path);

public slots:
    // trigerred only by sigPagesReady of PagesContainer widget - tells that pages are
    // rendered and ready to be displayed
    void slotDisplayPages();

private:
    PagesContainer* m_pPagesContainer;

    void cleanLayout();
};

