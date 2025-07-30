//
// Created by Purple-Rose on 7/26/2025.
//

#include "GND.h"
#include <QGraphicsScene>
#include <QPixmap>
GND::GND(QObject* parent) : QObject(parent), graphicItem(nullptr) {}

QGraphicsItem* GND::create(QGraphicsScene* scene) {
    QPixmap capacitorImage("../images/GND.png");
    capacitorImage = capacitorImage.scaled(80, 40, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(capacitorImage);
    graphicItem ->setData(0, QVariant::fromValue(this)); // ذخیره اشاره‌گر به خود کلاس

    updatePorts();
    return graphicItem;
}
void GND::updatePorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port = snapToGrid(graphicItem->mapToScene(QPointF(rect.center().x(), rect.top())));
    }
}
QPointF GND::snapToGrid(const QPointF& point) {
    int x = qRound(point.x() / gridSize) * gridSize;
    int y = qRound(point.y() / gridSize) * gridSize;
    return QPointF(x, y);
}
QPointF GND::getPort() const {return port;}