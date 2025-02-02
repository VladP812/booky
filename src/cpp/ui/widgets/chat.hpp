#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QSpacerItem>

class Chat : public QWidget {
public:
    Chat(QWidget* parent = nullptr);

    void addUserMessage(QWidget* message);
    void addAssistantMessage(QWidget* message);

private:
    QVBoxLayout* messageLayout;
};
