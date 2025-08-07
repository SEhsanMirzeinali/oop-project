#include "Inductor.h"
Inductor::Inductor(double l, const std::string& name, Node* n1, Node* n2)
    : Component(name, n1, n2), inductance(l) {}

std::string Inductor::getName() { return name; }
Node* Inductor::getNode1() { return node1; }
Node* Inductor::getNode2() { return node2; }

double Inductor::getVoltage() {
    return node1->getVoltage() - node2->getVoltage();
}

double Inductor::getCurrent() {
    return 0.0;
}

std::vector<double> Inductor::getTCurrent() {
    return TCurrent;
}

double Inductor::getInductance() {
    return inductance;
}
std::complex<double> Inductor::getImpedance(double omega) {
    const std::complex<double> j(0.0, 1.0);
    return  (j * omega * inductance);
}
void Inductor::setTCurrent(std::vector<double> TCurrent) {
    this->TCurrent = TCurrent;
}