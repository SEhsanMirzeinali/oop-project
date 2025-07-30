#include "GraphicComponent.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <utility>

int gridSize = 20; // تعریف متغیر global

GraphicComponent::GraphicComponent(QObject* parent) : QObject(parent), graphicItem(nullptr) {
   // QString prefix = getComponentPrefix();
    //componentName = prefix + QString::number(++componentCounters[prefix]);
}
GraphicComponent::~GraphicComponent() {
    if(graphicItem && graphicItem->scene()) {
        graphicItem->scene()->removeItem(graphicItem);
    }
}

void GraphicComponent::setupLabelInteractions() {
    // if(nameLabel) {
    //     nameLabel->setAcceptHoverEvents(true);
    //     nameLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //     nameLabel->setData(0, QVariant::fromValue(this));
    // }
    // if(valueLabel) {
    //     valueLabel->setAcceptHoverEvents(true);
    //     valueLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //     valueLabel->setData(0, QVariant::fromValue(this));
    // }
}
void GraphicComponent::initComponentName(const QString& prefix, int number) {
    componentName = prefix + QString::number(number);
}
void GraphicComponent::initComponentValue (const QString& prefix) {
    componentValue = (prefix) ;
}
void GraphicComponent::drawBoundingBox(QGraphicsScene* scene) {
    if (!graphicItem) return;

    static QGraphicsRectItem* boundingBox = nullptr;
    static QGraphicsEllipseItem* topPoint = nullptr;
    static QGraphicsEllipseItem* bottomPoint = nullptr;
    static QGraphicsEllipseItem* leftPoint = nullptr;
    static QGraphicsEllipseItem* rightPoint = nullptr;

    if (boundingBox) scene->removeItem(boundingBox);
    if (topPoint) scene->removeItem(topPoint);
    if (bottomPoint) scene->removeItem(bottomPoint);
    if (leftPoint) scene->removeItem(leftPoint);
    if (rightPoint) scene->removeItem(rightPoint);

    QRectF rect = graphicItem->boundingRect();
    QPointF pos = graphicItem->pos();
    boundingBox = scene->addRect(rect.translated(pos), QPen(Qt::blue, 1, Qt::DashLine));
    boundingBox->setZValue(15);

    QPointF center = rect.center() + pos;
    qreal radius = 3;

    QPointF top(center.x(), rect.top() + pos.y());
    topPoint = scene->addEllipse(top.x()-radius, top.y()-radius, radius*2, radius*2,
                                QPen(Qt::NoPen), QBrush(Qt::green));

    QPointF bottom(center.x(), rect.bottom() + pos.y());
    bottomPoint = scene->addEllipse(bottom.x()-radius, bottom.y()-radius, radius*2, radius*2,
                                  QPen(Qt::NoPen), QBrush(Qt::red));

    QPointF left(rect.left() + pos.x(), center.y());
    leftPoint = scene->addEllipse(left.x()-radius, left.y()-radius, radius*2, radius*2,
                                QPen(Qt::NoPen), QBrush(Qt::yellow));

    QPointF right(rect.right() + pos.x(), center.y());
    rightPoint = scene->addEllipse(right.x()-radius, right.y()-radius, radius*2, radius*2,
                                  QPen(Qt::NoPen), QBrush(Qt::cyan));

    topPoint->setZValue(20);
    bottomPoint->setZValue(20);
    leftPoint->setZValue(20);
    rightPoint->setZValue(20);
}

void GraphicComponent::updatePorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(QPointF(rect.right(), rect.center().y())));
        port2 = snapToGrid(graphicItem->mapToScene(QPointF(rect.left(), rect.center().y())));
    }
}
QGraphicsItem* GraphicComponent::getGraphicItem() const { return graphicItem; }
QPointF GraphicComponent::getPort1() const { return port1; }
QPointF GraphicComponent::getPort2() const { return port2; }
QString GraphicComponent::getComponentName() const{return componentName;}
QString GraphicComponent::getComponentValue() const{return componentValue;}
QGraphicsTextItem* GraphicComponent::getValueLabel() const{return valueLabel;}
QGraphicsTextItem* GraphicComponent::getNameLabel()const{return nameLabel;}
QString GraphicComponent::getNodeName1() const{return nodeName1;}
QString GraphicComponent::getNodeName2() const{return nodeName2;}

void GraphicComponent::setGraphicItem(QGraphicsPixmapItem* item) {graphicItem = item;}
void GraphicComponent::setComponentName(const QString& name)
{
    componentName = name;
    if (nameLabel) {
        nameLabel->setPlainText(componentName);
    }
}
void GraphicComponent::setComponentValue(const QString& val)
{
    componentValue = val;
    if (valueLabel) {
        valueLabel->setPlainText(componentValue);
    }
}
void GraphicComponent::setNodeName1(const QString &name){nodeName1=name;}
void GraphicComponent::setNodeName2(const QString &name){nodeName2=name;}


void GraphicComponent::updateLabelPosition(int angle)
{
    if (nameLabel && graphicItem) {
    QRectF rect = graphicItem->boundingRect();
        QPointF pos1;
        QPointF pos2 ;
        if ((angle / 90) % 4 == 0) {
             pos1 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()-45));
             pos2 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()+20));
        }
        else if ((angle / 90) % 4 == 1) {
            pos1 = graphicItem->mapToScene(QPointF(rect.center().x()-35, rect.center().y()-rect.width()/4));
            pos2 = graphicItem->mapToScene(QPointF(rect.center().x(), rect.center().y()-rect.width()/3));

        }
        else if ((angle / 90) % 4 == 2) {
            pos1 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()-20));
            pos2 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()+40));

        }
        else if ((angle / 90) % 4 == 3) {
            pos1 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()-40));
            pos2 = graphicItem->mapToScene(QPointF(rect.center().x()-10, rect.center().y()+20));

        }
    // محاسبه موقعیت مرکز آیتم گرافیکی
   // qreal centerX = pos.x() + rect.width() / 2;
    //qreal centerY = pos.y() - 10; // 20 پیکسل بالاتر از آیتم

    // تنظیم موقعیت با در نظر گرفتن اندازه متن
    //QRectF textRect = labelItem->boundingRect();
        nameLabel->setPos(pos1.x(), pos1.y());
        valueLabel->setPos(pos2.x(), pos2.y());

        nameLabel->setAcceptHoverEvents(true);
        valueLabel->setAcceptHoverEvents(true);

        // تنظیم داده برای تشخیص
        nameLabel->setData(0, QVariant::fromValue(this));
        valueLabel->setData(0, QVariant::fromValue(this));
    //labelItem->setPos(centerX - textRect.width() / 2, centerY);
}
}
QPointF GraphicComponent::snapToGrid(const QPointF& point) {
    int x = qRound(point.x() / gridSize) * gridSize;
    int y = qRound(point.y() / gridSize) * gridSize;
    return QPointF(x, y);
}

void GraphicComponent::setProperties(const QString& p1, const QString& p2) {
    property1 = p1;
    property2 = p2;
}

QString GraphicComponent::getProperty1() const {
    return property1;
}

QString GraphicComponent::getProperty2() const {
    return property2;
}