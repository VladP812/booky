#include "toptoolbar.hpp"
#include "mainwindow.hpp"
#include "documentview.hpp"

#include <QToolBar>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QFileDialog>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QApplication>
#include <QTimer>
#include <QThread>

MainWindow::MainWindow() : QMainWindow(nullptr){
    QWidget* mainWidget = new QWidget(this);
    //mainWidget->setStyleSheet("border: 1px dashed black;");
    QGridLayout* gridLayout = new QGridLayout(mainWidget);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    // persistent top toolbar 
    TopToolBar* topToolBar = new TopToolBar(this);
    QAction* fileMenuAction = new QAction("File", topToolBar);
    // menu which opens when File button is pressed on the top toolbar
    QMenu* fileMenu = new QMenu("FileMenu", topToolBar);
    topToolBar->addAction(fileMenuAction);
    // widget holding document's pages or welcome text if nothing is opened
    DocumentView* documentView = new DocumentView(mainWidget);
    documentView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QAction* openFileAction = new QAction("Open", topToolBar);
    fileMenu->addAction(openFileAction);

    gridLayout->addWidget(topToolBar, 0, 0, Qt::AlignLeft);
    gridLayout->addWidget(documentView, 1, 0);

    setCentralWidget(mainWidget);

    connect(fileMenuAction, &QAction::triggered, fileMenu,
            [=](){
                QString filePath = QFileDialog::getOpenFileName(
                                        this, tr("Open File"), QString(), QString(), 
                                        nullptr);
                if(filePath.isEmpty()) return;
                documentView->loadAndDisplayDocument(filePath.toStdString());
            });
}
