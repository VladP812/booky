#pragma once

#include "chat.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>

class AssistantWidget : public QWidget{
public:
    AssistantWidget(QWidget* parent = nullptr);

private:
    void sendMessage();
    void answerUserMessage(const QString& message);

    QLineEdit* messageInput;
    Chat* chat;
};
