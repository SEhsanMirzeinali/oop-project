//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef CURRENTSOURCE_H
#define CURRENTSOURCE_H


#include "..\Component.h"

class CurrentSource :public Component{
private:
    double current;
public:
    CurrentSource(double i,const std::string& name, Node* n1, Node* n2) : Component(name, n1, n2), current(i) {}
    std::string getName() override {return name;}
    Node* getNode1()override {return node1;}
    Node* getNode2() override {return node2;}
    double getVoltage() override {
        return node1->getVoltage()-node2->getVoltage();
    }
    double getCurrent() override {
        return current;
    }
    std::vector<double>  getTCurrent() override { std::vector<double> t; return t; }


};



#endif //CURRENTSOURCE_H
