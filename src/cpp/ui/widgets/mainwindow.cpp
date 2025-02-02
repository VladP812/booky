#include "assistant.hpp"
#include "toptoolbar.hpp"
#include "mainwindow.hpp"
#include "pagescontainer.hpp"
#include "../../threads/databasecreator.hpp"

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
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>

#define ASSISTANT_BUTTON_WIDTH 25
#define ASSISTANT_BUTTON_HEIGHT 45
#define ASSISTANT_BUTTON_OFFSET 45

MainWindow::MainWindow() : 
    QMainWindow(nullptr),
    assistantButton("<", this),
    infoTextLabel("Open a document to begin", this),
    pagesContainer(nullptr),
    pagesScrollArea(nullptr),
    isAssistantOpen(false),
    assistant(),
    stackOfWidgets(new QStackedWidget(this)),
    configurator([this](){switchToDocumentView();}, this)
{
    assistantButton.setFixedSize(ASSISTANT_BUTTON_WIDTH, ASSISTANT_BUTTON_WIDTH);
    assistantButton.hide();
    assistant.hide();

    connect(&assistantButton, &QPushButton::clicked,
            this, &MainWindow::toggleAssitant);
    
    QWidget* documentView = new QWidget(this);
    documentViewLayout = new QGridLayout(documentView);
    documentViewLayout->setContentsMargins(0, 0, 0, 0);
    documentViewLayout->setSpacing(0);
    documentViewLayout->addWidget(&infoTextLabel, 1, 0, Qt::AlignCenter);

    stackOfWidgets->addWidget(documentView);
    stackOfWidgets->addWidget(&configurator);

    TopToolBar* topToolBar = new TopToolBar(
            [this](std::string filePath){
                loadAndDisplayDocument(filePath);},
            [this](){
                switchToAppearanceSettings();
            },
                                        this);
    addToolBar(topToolBar);

    setCentralWidget(stackOfWidgets);
}

void MainWindow::switchToAppearanceSettings() {
    assistantButton.hide();
    stackOfWidgets->setCurrentIndex(1);
}

void MainWindow::switchToDocumentView() {
    assistantButton.show();
    stackOfWidgets->setCurrentIndex(0);
}

void MainWindow::loadAndDisplayDocument(std::string path) {
    infoTextLabel.setText("Processing the document...");
    pagesContainer = new PagesContainer(path, this);

    connect(pagesContainer, &PagesContainer::sigPagesReady,
            this, &MainWindow::slotDisplayPages);
    // creates pages pixmaps in a separate thread
    pagesContainer->processDocument();
    // creates a knowledgebase in a separate thread
    DatabaseCreatorThread* dbCreator = new DatabaseCreatorThread(path, this);
    connect(dbCreator, &QThread::finished, dbCreator, &QObject::deleteLater);

    dbCreator->start();
}

void MainWindow::slotDisplayPages() {
    infoTextLabel.hide();

    pagesScrollArea = new QScrollArea(this); 

    pagesScrollArea->setAlignment(Qt::AlignHCenter);
    pagesScrollArea->setWidgetResizable(true);
    pagesScrollArea->setWidget(pagesContainer);

    documentViewLayout->addWidget(pagesScrollArea, 1, 0);
    
    assistantButton.show();
    assistantButton.raise();
    QTimer::singleShot(0, this, &MainWindow::repositionOverlay);
}

void MainWindow::toggleAssitant() {
    if (!isAssistantOpen) {
        documentViewLayout->setColumnStretch(0, 1);
        documentViewLayout->setColumnStretch(1, 1);
        assistant.show();
        documentViewLayout->addWidget(&assistant, 1, 1);
        assistantButton.setText(">");
    }
    else {
        documentViewLayout->setColumnStretch(0, 0);
        documentViewLayout->setColumnStretch(1, 0);
        assistant.hide();
        documentViewLayout->removeWidget(&assistant);
        assistantButton.setText("<");
    }
    isAssistantOpen = !isAssistantOpen;
    QTimer::singleShot(0, this, &MainWindow::repositionOverlay);
}

// it is ugly now but will be reworked (eventually)
void MainWindow::repositionOverlay() {
    QPoint posForButton;
    if (isAssistantOpen) {
        QPoint assistantPos = assistant.mapTo(this, QPoint(0, 0));
        QSize assistantSize = assistant.size();
        posForButton = QPoint(assistantPos.x() + (ASSISTANT_BUTTON_OFFSET / 3), // weird but ok
                assistantPos.y() + (assistantSize.height() / 2) - (ASSISTANT_BUTTON_HEIGHT / 2));
    }
    else {
        QPoint pagesScrollAreaPos = pagesScrollArea->mapTo(this, QPoint(0, 0));
        QSize pagesScrollAreaSize = pagesScrollArea->size();
        posForButton = QPoint(
                pagesScrollAreaPos.x() + pagesScrollAreaSize.width() - ASSISTANT_BUTTON_OFFSET,
            pagesScrollAreaPos.y() + (pagesScrollAreaSize.height() / 2) - (ASSISTANT_BUTTON_HEIGHT / 2)
            );
    }
    assistantButton.move(posForButton);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    if (!pagesContainer || !pagesScrollArea) return;
    repositionOverlay();
}
