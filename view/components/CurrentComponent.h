#ifndef CURRENTCOMPONENT_H
#define CURRENTCOMPONENT_H

#include "graphiccomponent.h"

class CurrentComponent : public GraphicComponent {
    Q_OBJECT
public:
    explicit CurrentComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;

};

#endif //CURRENTCOMPONENT_H