#pragma once

#include <QLabel>
#include <QWidget>

class ChatMessage : public QLabel {
public:
    explicit ChatMessage(const QString& text, const QString& stylesheet, QWidget* parent = nullptr)
        : QLabel(parent)
    {
        setText(text);
        setStyleSheet(stylesheet);
        setWordWrap(true);
    }
};
