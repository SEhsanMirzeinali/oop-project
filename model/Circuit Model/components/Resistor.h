//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef RESISTOR_H
#define RESISTOR_H

#include "..\Component.h"

class Resistor :public Component{
private:
    double resistance;
    std::vector<double> tCurrent;
public:
    Resistor(double r,const std::string& name, Node* n1, Node* n2) : Component(name, n1, n2), resistance(r) {}
    std::string getName() override {return name;}
    Node* getNode1()override {return node1;}
    Node* getNode2() override {return node2;}
    double getVoltage() override {
        return node1->getVoltage()-node2->getVoltage();
    }
    double getCurrent() override {
        return node1->getVoltage()-node2->getVoltage()/resistance;
    }
    double getConductance() {
        return 1.0/resistance;
    }
    double getResistance(){return resistance;}

    void setTCurrent() {
        const std::vector<double>& v1=this->getNode1()->getTVoltage();
        const std::vector<double>& v2=this->getNode2()->getTVoltage();
        tCurrent.clear();
        for (size_t i = 0; i < v1.size(); ++i) {
            tCurrent.push_back((v1[i] - v2[i]) / resistance);
        }
    }
    std::vector<double>  getTCurrent() override { return tCurrent; }

};



#endif //RESISTOR_H
