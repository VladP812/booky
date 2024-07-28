#pragma once

#include <QMainWindow>
#include <QGridLayout>

#include <string>
#include <vector>

class PdfView: public QMainWindow {
    Q_OBJECT;

public:
    explicit PdfView(std::string filePath, QWidget* parent = nullptr);

protected:
    void onMousePress(QMousevent* event) override;
private:
    QWidget m_mainWidget;
};

