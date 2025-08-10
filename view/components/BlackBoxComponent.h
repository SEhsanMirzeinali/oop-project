//
// Created by Purple-Rose on 8/9/2025.
//

#ifndef BLACKBOXCOMPONENT_H
#define BLACKBOXCOMPONENT_H

#include "graphiccomponent.h"


class BlackBoxComponent : public GraphicComponent {
    Q_OBJECT

private:
    std::vector<std::string> netList;
public:
    explicit BlackBoxComponent(QObject* parent = nullptr);
    QGraphicsItem* create(QGraphicsScene* scene) override;
    void setPorts() override;
    QString getComponentPrefix() const override;
    void handleUnipolarNetList(std::vector<std::string> netList);
    std::vector<std::string> getNetList();
    std::vector<std::string> replaceCircuitNodes(std::vector<std::string> netList,std::string n1, std::string n2);

};




#endif //BLACKBOXCOMPONENT_H
