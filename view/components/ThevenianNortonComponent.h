//
// Created by Purple-Rose on 8/10/2025.
//

#ifndef THEVENIANNORTONCOMPONENT_H
#define THEVENIANNORTONCOMPONENT_H

#include "graphiccomponent.h"


class ThevenianNortonComponent : public GraphicComponent {
    Q_OBJECT

private:
    bool isThevenian=true;
    std::vector<std::string> netList;
    double Voc;
    double Isc;
    double Req;
    QGraphicsTextItem* RLabel = nullptr;
    QGraphicsTextItem* IOrVLabel = nullptr;

public:
    QGraphicsTextItem* getRLabel(){return RLabel;};
    QGraphicsTextItem* getIOrVLabel(){return IOrVLabel;};
    explicit ThevenianNortonComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;
    void handleUnipolarNetList(std::vector<std::string> netList);
    std::vector<std::string> getNetList();
    std::vector<std::string> replaceCircuitNodes(std::vector<std::string> netList,std::string n1, std::string n2);
    void setIsThevenian(bool isThevenian);
};

#endif //THEVENIANNORTONCOMPONENT_H
