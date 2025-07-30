//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "..\Component.h"

#include "..\Component.h"

class VoltageSource :public Component{
private:
    double voltage;
    double current;
    std::vector<double> TCurrent;
public:
    VoltageSource(double v,const std::string& name, Node* n1, Node* n2) : Component(name, n1, n2), voltage(v) {}
    std::string getName() override {return name;}
    Node* getNode1()override {return node1;}
    Node* getNode2() override {return node2;}
    double getVoltage() override {
        return voltage;
    }
    double getCurrent() override {
        return current;
    }
    std::vector<double>  getTCurrent() override { return TCurrent; }

    void setTCurrent(std::vector<double> c) {
        TCurrent = c;
    }

};



#endif //VOLTAGESOURCE_H
