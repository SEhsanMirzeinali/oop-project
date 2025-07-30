#include "CurrentSource.h"

CurrentSource::CurrentSource(double i, const std::string& name, Node* n1, Node* n2) 
    : Component(name, n1, n2), current(i) {}

std::string CurrentSource::getName() { return name; }
Node* CurrentSource::getNode1() { return node1; }
Node* CurrentSource::getNode2() { return node2; }

double CurrentSource::getVoltage() {
    return node1->getVoltage() - node2->getVoltage();
}

double CurrentSource::getCurrent() {
    return current;
}

std::vector<double> CurrentSource::getTCurrent() {
    return std::vector<double>();
}