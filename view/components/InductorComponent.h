#ifndef INDUCTORCOMPONENT_H
#define INDUCTORCOMPONENT_H

#include "graphiccomponent.h"

class InductorComponent : public GraphicComponent {
    Q_OBJECT
public:
    explicit InductorComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;

};

#endif // INDUCTORCOMPONENT_H