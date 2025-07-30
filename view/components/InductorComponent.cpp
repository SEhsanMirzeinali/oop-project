#include "InductorComponent.h"
#include <QGraphicsScene>
#include <QPixmap>

InductorComponent::InductorComponent(QObject* parent) : GraphicComponent(parent) {
    static int inductorCounter = 0;
    initComponentName("L", ++inductorCounter);
    initComponentValue("L");

}

QGraphicsItem* InductorComponent::create(QGraphicsScene* scene) {
    QPixmap inductorImage("../images/Inductor.png");
    inductorImage = inductorImage.scaled(160, 80, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(inductorImage);
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

void InductorComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString InductorComponent::getComponentPrefix() const
{
    return "L";
}