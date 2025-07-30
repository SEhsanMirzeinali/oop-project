#include "WireComponent.h"
#include <QPainter>
#include <QGraphicsScene>
#include <iostream>

int WireComponent::wireCounter = 0;

WireComponent::WireComponent(QObject* parent)
    : QObject(parent), QGraphicsPathItem() {
    wireName = "n00" + QString::number(++wireCounter);
    setPen(QPen(Qt::black, 2));
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

WireComponent::~WireComponent() {}

QString WireComponent::getWireName() const
{
    return wireName;
}
bool WireComponent::getIsGnd() const {
    return isGnd;
}

void WireComponent::setWireName(const QString& name)
{
    wireName = name;
    if (labelItem) {
        labelItem->setPlainText(wireName);
    }
}

void WireComponent::updateLabelPosition()
{
    if (labelItem && !points.isEmpty()) {
        QPointF midPoint = (points.first() + points.last()) / 2;
        labelItem->setPos(midPoint);
    }
}

void WireComponent::setConnectedComponent(QVector<GraphicComponent*> components) {
    connectedComponents.append(components);
}

void WireComponent::addConnectedComponent(GraphicComponent* component) {
    if (component && !connectedComponents.contains(component)) {
        connectedComponents.append(component);
    }
}

const QVector<GraphicComponent*>& WireComponent::getConnectedComponents() const {
    return connectedComponents;
}

void WireComponent::resetPoints(const QList<QPointF>& newPoints) {
    points = newPoints;
    updatePath();
}

WireComponent* WireComponent::clone(QObject* parent) {
    WireComponent* newWire = new WireComponent(parent);
    newWire->points = this->points;
    newWire->allPoints = this->allPoints;
    newWire->connectedComponents = this->connectedComponents;
    newWire->setPath(this->path());
    newWire->setPen(this->pen());
    if (this->scene()) {
        this->scene()->addItem(newWire);
    }
    return newWire;
}

void WireComponent::mergeWire(WireComponent* other) {
    if (!other || other ==this) return;

    if (!points.isEmpty() && !other->points.isEmpty() &&
        points.first() == other->points.last()) {
        points = other->points + points;
    }
    else if (!points.isEmpty() && !other->points.isEmpty() &&
             points.last() == other->points.first()) {
        points += other->points;
    }
    else {
        points += other->points;
    }

    setAllPoints();
    updatePath();
    isGnd = other->getIsGnd();
    for(int i=0; i < other->getConnectedComponents().size(); i++) {
        bool exist=false;
        for(int j=0; j<connectedComponents.size(); j++) {
            if(other->getConnectedComponents()[i] == connectedComponents[j]) {
                exist=true;
                break;
            }
        }
        if(!exist) {
            connectedComponents.push_back(other->getConnectedComponents()[i]);
        }
    }

}

void WireComponent::updatePath() {
    QPainterPath path;
    QList<QList<QPointF>> segments;
    QList<QPointF> currentSegment;

    if (!points.isEmpty()) {
        currentSegment.append(points.first());

        for (int i = 1; i < points.size(); ++i) {
            if (shouldConnect(points[i-1], points[i])) {
                currentSegment.append(points[i]);
            } else {
                segments.append(currentSegment);
                currentSegment.clear();
                currentSegment.append(points[i]);
            }
        }

        segments.append(currentSegment);
    }

    for (const auto& segment : segments) {
        if (!segment.isEmpty()) {
            path.moveTo(segment.first());
            for (int i = 1; i < segment.size(); ++i) {
                path.lineTo(segment[i]);
            }
        }
    }

    setPath(path);
}

bool WireComponent::shouldConnect(const QPointF& p1, const QPointF& p2) {
    return qFuzzyCompare(p1.x(), p2.x()) || qFuzzyCompare(p1.y(), p2.y());
}

void WireComponent::printPoints() {
    for(int i = 0; i < allPoints.size(); i++) {
        std::cout << i+1 << ": (" << allPoints[i].x() << ", " << allPoints[i].y() << ")\n";
    }
}

void WireComponent::addNewPath(const QList<QPointF>& newPoints) {
    subPaths.append(newPoints);
    updatePath();
}

void WireComponent::addPoint(const QPointF& point) {
    points << point;
    updatePath();
}

void WireComponent::clearPoints() {
    points.clear();
    updatePath();
}

const QList<QPointF>& WireComponent::getPoints() const {
    return points;
}

const QList<QPointF>& WireComponent::getAllPoints() const {
    return allPoints;
}

void WireComponent::printAllPoints() {
    for (int i = 0; i < allPoints.size(); ++i) {
        std::cout<<" X: "<<allPoints[i].x()<<"     Y: "<<allPoints[i].y()<<std::endl;
    }
}

void WireComponent::setAllPoints() {
    allPoints.clear();
    if (points.isEmpty()) return;

    QPointF prePoint = points[0];
    allPoints.push_back(prePoint);

    for (int i = 1; i < points.size(); ++i) {
        QPointF currentPoint = points[i];

        if (qFuzzyCompare(prePoint.x(), currentPoint.x())) {
            int x = static_cast<int>(prePoint.x());
            int y1 = static_cast<int>(prePoint.y());
            int y2 = static_cast<int>(currentPoint.y());

            int step = (y2 > y1) ? gridSize : -gridSize;
            for (int y = y1 + step; (step > 0) ? (y <= y2) : (y >= y2); y += step) {
                allPoints.push_back(QPointF(x, y));
            }
        }
        else if (qFuzzyCompare(prePoint.y(), currentPoint.y())) {
            int y = static_cast<int>(prePoint.y());
            int x1 = static_cast<int>(prePoint.x());
            int x2 = static_cast<int>(currentPoint.x());

            int step = (x2 > x1) ? gridSize : -gridSize;
            for (int x = x1 + step; (step > 0) ? (x <= x2) : (x >= x2); x += step) {
                allPoints.push_back(QPointF(x, y));
            }
        }
        else {
            allPoints.push_back(currentPoint);
        }

        prePoint = currentPoint;
    }
}
void WireComponent::setAsGnd(bool b) {
    isGnd = b;
    if(isGnd) {
        wireName="0";
    }
}