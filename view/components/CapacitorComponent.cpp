#include "CapacitorComponent.h"
#include <QGraphicsScene>
#include <QPixmap>

CapacitorComponent::CapacitorComponent(QObject* parent) : GraphicComponent(parent) {
    static int capacitorCounter = 0;
        initComponentName("C", ++capacitorCounter);
        initComponentValue("C");

}

QGraphicsItem* CapacitorComponent::create(QGraphicsScene* scene) {
    QPixmap capacitorImage("../images/Capacitor.png");
    capacitorImage = capacitorImage.scaled(80, 40, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(capacitorImage);
    graphicItem->setData(0, QVariant::fromValue(this));

    nameLabel = scene->addText(componentName);
    valueLabel = scene->addText(componentValue);

    QFont font("Arial", 12, QFont::Bold); // فونت Arial با سایز 10 و حالت Bold
    //font.setItalic(true); // حالت ایتالیک (اختیاری)
    valueLabel->setFont(font);
    valueLabel->setZValue(20);
    nameLabel->setFont(font);
    nameLabel->setZValue(20);
    updateLabelPosition();

    updatePorts();
    return graphicItem;
}

void CapacitorComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString CapacitorComponent::getComponentPrefix() const
{
    return "C";
}