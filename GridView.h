#pragma once

#include <QWidget>

class GridViewDelegate
{
public:
    virtual ~GridViewDelegate() = default;
    virtual QSize cellSize() const = 0;
    virtual QSize gridSize() const = 0;
    virtual void drawCell(QPainter* painter, const QRect& rect, int column, int row) = 0;
};

class GridView : public QWidget
{
public:
    using QWidget::QWidget;

    void setDelegate(GridViewDelegate* delegate);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    GridViewDelegate* _delegate = nullptr;
};
