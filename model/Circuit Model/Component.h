//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include"Node.h"

class Component {
public:
    Component(const std::string &n, Node* n1, Node* n2) : name(n), node1(n1), node2(n2) {}

    virtual double getVoltage()=0;
    virtual double getCurrent()=0;
    virtual std::string getName()=0;
    virtual Node *getNode1()=0;
    virtual Node *getNode2()=0;
    virtual std::vector<double> getTCurrent()=0;

protected:
    int id;
    std::string name;
    Node* node1;
    Node* node2; // Connection points
};



#endif //COMPONENT_H
