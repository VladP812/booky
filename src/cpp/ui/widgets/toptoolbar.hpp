#pragma once

#include <functional>

#include <QToolBar>

class TopToolBar : public QToolBar {
public:
    explicit TopToolBar(const std::function<void(std::string)>& onOpenDocument,
                        const std::function<void()>& onAppearancePressed,
                       QWidget* parent = nullptr);
};
