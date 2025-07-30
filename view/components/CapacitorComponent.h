#ifndef CAPACITORCOMPONENT_H
#define CAPACITORCOMPONENT_H

#include "graphiccomponent.h"

class CapacitorComponent : public GraphicComponent {
    Q_OBJECT
public:
    explicit CapacitorComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;

};

#endif // CAPACITORCOMPONENT_H