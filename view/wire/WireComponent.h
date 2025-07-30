#ifndef WIRECOMPONENT_H
#define WIRECOMPONENT_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QList>
#include <QVector>
#include "../components/GraphicComponent.h"

class WireComponent : public QObject, public QGraphicsPathItem {
    Q_OBJECT
private:
    QList<QPointF> points;
    QList<QPointF> allPoints;
    QList<QList<QPointF>> subPaths;
    QVector<GraphicComponent*> connectedComponents;
    QString wireName;
    QGraphicsTextItem* labelItem = nullptr;
    static int wireCounter;
    bool isGnd;
public:
    explicit WireComponent(QObject* parent = nullptr);
    ~WireComponent() override;

    QString getWireName() const;
    bool getIsGnd() const;
    void setWireName(const QString& name);
    void updateLabelPosition();
    void setConnectedComponent(QVector<GraphicComponent*> components);
    void addConnectedComponent(GraphicComponent* component);
    const QVector<GraphicComponent*>& getConnectedComponents() const;
    void resetPoints(const QList<QPointF>& newPoints);
    WireComponent* clone(QObject* parent = nullptr);
    void mergeWire(WireComponent* other);
    void updatePath();
    bool shouldConnect(const QPointF& p1, const QPointF& p2);
    void printPoints();
    void addNewPath(const QList<QPointF>& newPoints);
    void addPoint(const QPointF& point);
    void clearPoints();
    const QList<QPointF>& getPoints() const;
    const QList<QPointF>& getAllPoints() const;
    void printAllPoints();
    void setAllPoints();
    void setAsGnd(bool isGnd);
};

#endif // WIRECOMPONENT_H