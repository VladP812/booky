#pragma once

#include <QThread>
#include <QString>

class AssistantResponseGenerator : public QThread {
    Q_OBJECT;
public:
    AssistantResponseGenerator(QString userMessage, bool useKnowledgebase,
                               QObject* parent = nullptr);

    void run() override;

signals:
    void responseGenerated(QString response);

private:
    QString query;
    bool useKnowledgebase;
};
