#include "assistant.hpp"
#include "chat.hpp"
#include "userchatmessage.hpp"
#include "assistantchatmessage.hpp"
#include "../../threads/assistantresponsegenerator.hpp"
#include "messageinput.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

AssistantWidget::AssistantWidget(QWidget* parent):
    messageInput(new MessageInput(this)),
    useKnowledgebase(new QCheckBox("Use knowledgebase", this)),
    chat(new Chat(this))
{    
    chat->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QGridLayout* gridLayout= new QGridLayout(this);
    gridLayout->setAlignment(Qt::AlignCenter);
    gridLayout->setContentsMargins(0, 0, 0, 5);
    gridLayout->setColumnStretch(0, 1);

    QPushButton* sendButton = new QPushButton("Send", this);
    connect(sendButton, &QPushButton::clicked,
            this, &AssistantWidget::sendMessage);
    connect(messageInput, &MessageInput::returnPressed,
            this, &AssistantWidget::sendMessage);
    
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(chat);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 2);
    gridLayout->addWidget(useKnowledgebase, 1, 1);
    gridLayout->addWidget(sendButton, 2, 1);
    gridLayout->addWidget(messageInput, 1, 0, 2, 1);
}

void AssistantWidget::sendMessage() {
    QString messageText = messageInput->toPlainText();
    if (messageText.isEmpty()) return;
    UserChatMessage* userMessage = new UserChatMessage(messageText, this);

    chat->addUserMessage(userMessage);
    answerUserMessage(messageText);
    messageInput->clear();
}

void AssistantWidget::answerUserMessage(const QString& message) {
    AssistantResponseGenerator* responseGenerator = new AssistantResponseGenerator(message,
                                                            useKnowledgebase->isChecked(), this);
    connect(responseGenerator, &AssistantResponseGenerator::responseGenerated,
            this, [this](QString response)
            {
                AssistantChatMessage* assistantAnswer = new AssistantChatMessage(response, this);
                chat->addAssistantMessage(assistantAnswer);
            },
            Qt::QueuedConnection);
    connect(responseGenerator, &QThread::finished, responseGenerator, &QObject::deleteLater);
    responseGenerator->start();
}
