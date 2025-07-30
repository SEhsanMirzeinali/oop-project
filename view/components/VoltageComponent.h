#ifndef VOLTAGECOMPONENT_H
#define VOLTAGECOMPONENT_H

#include "graphiccomponent.h"

class VoltageComponent : public GraphicComponent {
    Q_OBJECT
public:
    explicit VoltageComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;

};

#endif // VOLTAGECOMPONENT_H