#include "VoltageSource.h"

VoltageSource::VoltageSource(double v, const std::string& name, Node* n1, Node* n2) 
    : Component(name, n1, n2), voltage(v) {}

std::string VoltageSource::getName() { return name; }
Node* VoltageSource::getNode1() { return node1; }
Node* VoltageSource::getNode2() { return node2; }

double VoltageSource::getVoltage() {
    return voltage;
}

double VoltageSource::getCurrent() {
    return current;
}

std::vector<double> VoltageSource::getTCurrent() {
    return TCurrent;
}

void VoltageSource::setTCurrent(std::vector<double> c) {
    TCurrent = c;
}