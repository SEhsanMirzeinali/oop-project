#include "grid.h"
#include <QPainter>

Grid::Grid(int gridSize, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_gridSize(gridSize) {
    setZValue(-1);
}

QRectF Grid::boundingRect() const {
    return QRectF(-10000, -10000, 20000, 20000);
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QPen(Qt::lightGray, 0.5));

    for (int x = -10000; x <= 10000; x += m_gridSize) {
        painter->drawLine(x, -10000, x, 10000);
    }

    for (int y = -10000; y <= 10000; y += m_gridSize) {
        painter->drawLine(-10000, y, 10000, y);
    }
}