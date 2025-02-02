#pragma once

#include "chat.hpp"
#include "messageinput.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>

class AssistantWidget : public QWidget{
public:
    AssistantWidget(QWidget* parent = nullptr);

private:
    void sendMessage();
    void answerUserMessage(const QString& message);

    MessageInput* messageInput;
    QCheckBox* useKnowledgebase;
    Chat* chat;
};
