#ifndef RESISTORCOMPONENT_H
#define RESISTORCOMPONENT_H

#include "graphiccomponent.h"

class ResistorComponent : public GraphicComponent {
    Q_OBJECT
public:
    explicit ResistorComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;

};

#endif // RESISTORCOMPONENT_H