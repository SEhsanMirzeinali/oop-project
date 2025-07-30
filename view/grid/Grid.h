#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>

class Grid : public QGraphicsItem {
public:
    Grid(int gridSize = 20, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int m_gridSize;
};

#endif // GRID_H