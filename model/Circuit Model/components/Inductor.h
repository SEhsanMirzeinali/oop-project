//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef INDUCTOR_H
#define INDUCTOR_H



#pragma once
#include "../Component.h"

class Inductor :public Component{
private:
    double inductance;
    std::vector<double> TCurrent;
public:
    Inductor(double l,const std::string& name, Node* n1, Node* n2) : Component(name, n1, n2), inductance(l) {};
    std::string getName() override {return name;}
    Node* getNode1()override {return node1;}
    Node* getNode2() override {return node2;}
    double getVoltage() override {
        return node1->getVoltage()-node2->getVoltage();
    }
    double getCurrent() override {
return 0.0;
    }
    std::vector<double>  getTCurrent() override { return TCurrent; }

    double getInductance() {
        return inductance;
    }
    void setTCurrent(std::vector<double> TCurrent) {
        this->TCurrent = TCurrent;
    }
};



#endif //INDUCTOR_H
