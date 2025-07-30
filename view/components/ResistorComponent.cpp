#include "ResistorComponent.h"
#include <QGraphicsScene>
#include <QPixmap>

ResistorComponent::ResistorComponent(QObject* parent) : GraphicComponent(parent) {
    static int resistorCounter = 0;
    initComponentName("R", ++resistorCounter);
    initComponentValue("R");

}

QGraphicsItem* ResistorComponent::create(QGraphicsScene* scene) {
    QPixmap resistorImage("../images/Resistor.png");
    resistorImage = resistorImage.scaled(80, 80, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(resistorImage);
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

void ResistorComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString ResistorComponent::getComponentPrefix() const
{
    return "R";
}