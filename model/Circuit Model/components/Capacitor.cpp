#define _USE_MATH_DEFINES // برای M_PI (اختیاری)

#include "Capacitor.h"

#include <complex.h>

Capacitor::Capacitor(double c, const std::string& name, Node* n1, Node* n2) 
    : Component(name, n1, n2), capacity(c) {}

std::string Capacitor::getName() { return name; }
Node* Capacitor::getNode1() { return node1; }
Node* Capacitor::getNode2() { return node2; }

double Capacitor::getVoltage() {
    return node1->getVoltage() - node2->getVoltage();
}

double Capacitor::getCurrent() {
    // Implementation needed
    return 0.0;
}

double Capacitor::getCapacity() {
    return capacity;
}
std::complex<double> Capacitor::getImpedance(double omega) {
        const std::complex<double> j(0.0, 1.0);
        return 1.0 / (j * omega * capacity);
    }
void Capacitor::setTCurrent(double dt) {
    const std::vector<double>& v1 = this->getNode1()->getTVoltage();
    const std::vector<double>& v2 = this->getNode2()->getTVoltage();
    tCurrent.clear();
    
    for (size_t i = 1; i < v1.size(); ++i) {
        double dv = (v1[i] - v2[i]) - (v1[i-1] - v2[i-1]);
        tCurrent.push_back(capacity * dv / dt);
    }
    tCurrent.insert(tCurrent.begin(), 0.0);
}

std::vector<double> Capacitor::getTCurrent() {
    return tCurrent;
}