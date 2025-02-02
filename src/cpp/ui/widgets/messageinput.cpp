#include "messageinput.hpp"

MessageInput::MessageInput(QWidget* parent)
    : QTextEdit(parent)
{
    setPlaceholderText(tr("Type here"));
    setStyleSheet("QTextEdit { color: white; } "
                  "QTextEdit::placeholderText { color: gray; }");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MessageInput::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Return) {
        emit returnPressed();
        return;
    }
    QTextEdit::keyPressEvent(e);
}
