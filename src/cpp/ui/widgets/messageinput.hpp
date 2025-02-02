#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class MessageInput : public QTextEdit {
    Q_OBJECT;
public:
    MessageInput(QWidget* parent = nullptr);

signals:
    void returnPressed();

protected:
    void keyPressEvent(QKeyEvent* e) override;
};
