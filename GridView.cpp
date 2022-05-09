#include "GridView.h"

#include <QPainter>

constexpr int Spacing = 5;

void GridView::setDelegate(GridViewDelegate* delegate)
{
    _delegate = delegate;

    update();
}

void GridView::paintEvent(QPaintEvent *)
{
    if (!_delegate)
    {
        return;
    }

    QPainter painter(this);

    const auto gridSize = _delegate->gridSize();
    const auto cellSize = _delegate->cellSize();

    QSize size{cellSize.width() * gridSize.width() + Spacing * (gridSize.width() + 1),
               cellSize.height() * gridSize.height() + Spacing * (gridSize.height() + 1)};

    if (size.width() > width() || size.height() > height())
    {
        size.scale(width(), height(), Qt::KeepAspectRatio);
    }

    const auto offsetX = (width() - size.width()) / 2;
    const auto offsetY = (height() - size.height()) / 2;

    painter.fillRect(QRect({offsetX, offsetY}, size), Qt::white);

    const auto tileWidth = (size.width() - Spacing * (gridSize.width() + 1)) / gridSize.width();
    const auto tileHeight = (size.height() - Spacing * (gridSize.height() + 1)) / gridSize.height();

    for (auto row = 0; row < gridSize.height(); ++row)
    {     
        const auto y = offsetY + (tileHeight + Spacing) * row + Spacing;

        for (auto col = 0; col < gridSize.width(); ++col)
        {
            const QRect rect(offsetX + (tileWidth + Spacing) * col + Spacing, y, tileWidth, tileHeight);
            _delegate->drawCell(&painter, rect, col, row);
        }
    }
}
