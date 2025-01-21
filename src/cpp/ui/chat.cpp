#include "chat.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>

Chat::Chat(QWidget* parent):
    QWidget(parent),
    messageLayout(new QVBoxLayout(this))
{}

void Chat::addUserMessage(QWidget* message) {
    QLayoutItem* spacer = messageLayout->takeAt(messageLayout->count() - 1);
    //if (spacer) delete spacer;
    messageLayout->addWidget(message);
    messageLayout->addStretch();
}

void Chat::addAssistantMessage(QWidget* message) {
    QLayoutItem* spacer = messageLayout->takeAt(messageLayout->count() - 1);
    //if (spacer) delete spacer;
    messageLayout->addWidget(message);
    messageLayout->addStretch();
}
