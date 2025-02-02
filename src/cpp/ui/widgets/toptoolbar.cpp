#include "toptoolbar.hpp"

#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

TopToolBar::TopToolBar(const std::function<void(std::string)>& onOpenDocument,
                       const std::function<void()>& onAppearancePressed,
                       QWidget* parent) : QToolBar(parent) {
    QToolButton* fileButton = new QToolButton(this);
    fileButton->setText("File");
    fileButton->setPopupMode(QToolButton::InstantPopup);

    QMenu* fileMenu = new QMenu("FileMenu", this);
    fileButton->setMenu(fileMenu);

    addWidget(fileButton);

    QAction* openFileAction = new QAction("Open", fileMenu);
    fileMenu->addAction(openFileAction);

    connect(openFileAction, &QAction::triggered,
            [this, onOpenDocument](){
            QString filePath = QFileDialog::getOpenFileName(
                    this, tr("Open Document"), QString(), QString(), 
                    nullptr);
            if(filePath.isEmpty()) return;
            onOpenDocument(filePath.toStdString());
            });

    QToolButton* configButton = new QToolButton(this);
    configButton->setText("Configure");
    configButton->setPopupMode(QToolButton::InstantPopup);

    QMenu* configMenu = new QMenu("ConfigMenu", this);
    configButton->setMenu(configMenu);

    addWidget(configButton);

    QAction* appearanceMenuAction = new QAction("Appearance", configMenu);
    configMenu->addAction(appearanceMenuAction);

    connect(appearanceMenuAction, &QAction::triggered,
            onAppearancePressed);
}
