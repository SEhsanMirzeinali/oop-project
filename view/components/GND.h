//
// Created by Purple-Rose on 7/26/2025.
//

#ifndef GND_H
#define GND_H

#include <QObject>
#include <QGraphicsItem>
#include <QPointF>
#include <iostream>
extern int gridSize; // اندازه گرید

class GND : public QObject{
    Q_OBJECT
private:
    QPointF port;
    QGraphicsItem* graphicItem;
public:
    explicit GND(QObject* parent = nullptr);

    void updatePorts();
    void setPorts() ;
    QGraphicsItem* create(QGraphicsScene* scene);

    QPointF getPort() const;
    QGraphicsItem* getGraphicItem() const;
    QPointF snapToGrid(const QPointF& point);

};



#endif //GND_H
