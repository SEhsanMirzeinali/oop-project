#include "VoltageComponent.h"
#include <QGraphicsScene>
#include <QPixmap>

VoltageComponent::VoltageComponent(QObject* parent) : GraphicComponent(parent) {
    static int voltageCounter = 0;
    initComponentName("V", ++voltageCounter);
    initComponentValue("V");

}

QGraphicsItem* VoltageComponent::create(QGraphicsScene* scene) {
    QPixmap voltageImage("../images/VoltageSource.png");
    voltageImage = voltageImage.scaled(80, 80, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(voltageImage);
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

void VoltageComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString VoltageComponent::getComponentPrefix() const
{
    return "V";
}