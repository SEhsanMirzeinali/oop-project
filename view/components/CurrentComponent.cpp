//
// Created by Purple-Rose on 8/8/2025.
//

#include "CurrentComponent.h"
#include <QGraphicsScene>
#include <QPixmap>

CurrentComponent::CurrentComponent(QObject* parent) : GraphicComponent(parent) {
    static int currentCounter = 0;
    initComponentName("I", ++currentCounter);
    initComponentValue("I");

}

QGraphicsItem* CurrentComponent::create(QGraphicsScene* scene) {
    QPixmap currentImage("../images/CurrentSource.png");
    currentImage = currentImage.scaled(80, 80, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(currentImage);
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

void CurrentComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString CurrentComponent::getComponentPrefix() const
{
    return "I";
}