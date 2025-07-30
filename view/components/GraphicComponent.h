#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPointF>
#include <iostream>

extern int gridSize; // اندازه گرید

class GraphicComponent : public QObject {
    Q_OBJECT

protected:
    QPointF port1;
    QPointF port2;
    QGraphicsItem* graphicItem;
    QString componentName;
    QString componentValue;
    //QGraphicsTextItem* labelItem = nullptr;

    QGraphicsTextItem* nameLabel = nullptr;
    QGraphicsTextItem* valueLabel = nullptr;

    QString property1;
    QString property2;

    QString nodeName1;
    QString nodeName2;

    //static QMap<QString, int> componentCounters; // برای شمارش کامپوننت‌های هر نوع
    void initComponentName(const QString& prefix, int number);
    void initComponentValue(const QString& diffult);
    void setupLabelInteractions();

public:
    explicit GraphicComponent(QObject* parent = nullptr);
    virtual ~GraphicComponent();

    virtual void updatePorts();
    virtual void setPorts() = 0;
    virtual QGraphicsItem* create(QGraphicsScene* scene) = 0;
    virtual QString getComponentPrefix() const = 0;

    QGraphicsItem* getGraphicItem() const;
    QGraphicsTextItem* getNameLabel() const;
    QGraphicsTextItem* getValueLabel() const;

    QPointF getPort1() const;
    QPointF getPort2() const;
    QString getComponentName() const;
    QString getComponentValue() const;
    QString getNodeName1() const;
    QString getNodeName2() const;


    void setGraphicItem(QGraphicsPixmapItem* item);
    void setComponentName(const QString& name);
    void setComponentValue(const QString& val);
    void setNodeName1(const QString& name);
    void setNodeName2(const QString& name);


    void drawBoundingBox(QGraphicsScene* scene);
    void updateLabelPosition(int angel =0);

    QPointF snapToGrid(const QPointF& point);

    void setProperties(const QString& p1, const QString& p2);
    QString getProperty1() const;
    QString getProperty2() const;
};

#endif // GRAPHICCOMPONENT_H