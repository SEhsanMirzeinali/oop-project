//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef CAPACITOR_H
#define CAPACITOR_H

#pragma once
#include "../Component.h"

#include "..\Component.h"

class Capacitor :public Component{
private:
    double capacity;
    std::vector<double> tCurrent;
public:
    Capacitor(double c,const std::string& name, Node* n1, Node* n2) : Component(name, n1, n2), capacity(c) {};
    std::string getName() override {return name;}
    Node* getNode1()override {return node1;}
    Node* getNode2() override {return node2;}
    double getVoltage() override {
        return node1->getVoltage()-node2->getVoltage();
    }
    double getCurrent() override {
        //return abs(node1->getVoltage()-node2->getVoltage())/resistance;
    }
    double getCapacity() {
        return capacity;
    }

    void setTCurrent(double dt) {
        const std::vector<double>& v1=this->getNode1()->getTVoltage();
        const std::vector<double>& v2 = this->getNode2()->getTVoltage();
        tCurrent.clear();
        for (size_t i = 1; i < v1.size(); ++i) {
            double dv = (v1[i] - v2[i]) - (v1[i - 1] - v2[i - 1]);
            tCurrent.push_back(capacity * dv / dt);
        }
        tCurrent.insert(tCurrent.begin(), 0.0);
    }

     std::vector<double>  getTCurrent() override { return tCurrent; }
};



#endif //CAPACITOR_H
