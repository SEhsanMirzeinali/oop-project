#include "Resistor.h"

Resistor::Resistor(double r, const std::string& name, Node* n1, Node* n2) 
    : Component(name, n1, n2), resistance(r) {}

std::string Resistor::getName() { return name; }
Node* Resistor::getNode1() { return node1; }
Node* Resistor::getNode2() { return node2; }

double Resistor::getVoltage() {
    return node1->getVoltage() - node2->getVoltage();
}

double Resistor::getCurrent() {
    return (node1->getVoltage() - node2->getVoltage()) / resistance;
}

double Resistor::getConductance() {
    return 1.0 / resistance;
}

double Resistor::getResistance() {
    return resistance;
}

void Resistor::setTCurrent() {
    const std::vector<double>& v1 = this->getNode1()->getTVoltage();
    const std::vector<double>& v2 = this->getNode2()->getTVoltage();
    tCurrent.clear();
    
    for (size_t i = 0; i < v1.size(); ++i) {
        tCurrent.push_back((v1[i] - v2[i]) / resistance);
    }
}

std::vector<double> Resistor::getTCurrent() {
    return tCurrent;
}