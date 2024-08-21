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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QWidget m_mainWidget;
};
