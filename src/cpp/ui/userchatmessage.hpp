#pragma once

#include "chatmessage.hpp"

class UserChatMessage : public ChatMessage {
public:
    explicit UserChatMessage(QString text, QWidget* parent = nullptr) 
        : ChatMessage(text, UserChatMessage::stylesheet, parent) {};
private:
    inline const static QString stylesheet = "background-color: rgba(0, 0, 125, 0.3);"
                                             "border-radius: 10px;"
                                             "padding: 5px;";
};
