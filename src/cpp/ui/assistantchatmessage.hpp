#pragma once

#include "chatmessage.hpp"

class AssistantChatMessage : public ChatMessage {
public:
    explicit AssistantChatMessage(QString text, QWidget* parent = nullptr) 
        : ChatMessage(text, AssistantChatMessage::stylesheet, parent) {}
private:
    inline const static QString stylesheet = "background-color: rgba(0, 125, 0, 0.3);"
                                             "border-radius: 10px;"
                                             "padding: 5px;";
};
