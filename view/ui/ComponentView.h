#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

#include <QGraphicsView>
#include <QVector>
#include "../components/GraphicComponent.h"
#include "../wire/WireComponent.h"
#include"NetListHandler.h"

class ComponentView : public QGraphicsView {
    Q_OBJECT
private:
    QVector<GraphicComponent*> allComponents;
    QGraphicsItem* currentComponent;
    QString componentType;
    bool placing;
    bool wiring;
    QVector<WireComponent*> wires;
    WireComponent* currentWire;
    QList<QPointF> clickedPoints;
    QGraphicsLineItem* crosshairV;
    QGraphicsLineItem* crosshairH;
    bool isOverComponent = false; // اضافه کردن این متغیر
    QCursor componentCursor;
    QGraphicsItem* lastHoveredItem = nullptr;
    NetListHandler* netList;


    bool isComponent(QGraphicsItem* item) const;
    void updateHoverState(QGraphicsItem* item, const QPointF& scenePos);

    void drawPortMarkers(GraphicComponent* comp);
    GraphicComponent* findComponentAtPoint(const QPointF& point);
    void updateCrosshair(const QPointF& pos);
    QPointF snapToGrid(const QPointF& point);
    void endWiring();
    WireComponent* findWireContainingPoint(const QPointF& point);
    void finishCurrentWire(bool addToVector = true);
    void mergeWires(WireComponent* wire1, WireComponent* wire2);
    // در بخش private این متد را اضافه کنید:
    void showComponentPropertiesDialog(QGraphicsItem* comp);
public:
    ComponentView(QGraphicsScene* scene, QWidget* parent = nullptr);
    void startWiring();
    void startPlacing(const QString& type);
    std::vector<std::string> createNetList(QVector<GraphicComponent*>);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override; // اضافه کردن این خط
    void setGndWire(QGraphicsItem* currentComp);
    void setComponentsWires();

};

#endif // COMPONENTVIEW_H