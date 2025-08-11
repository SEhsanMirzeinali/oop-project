#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "../Component.h"
class Capacitor : public Component {
private:
    double capacity;
    std::vector<double> tCurrent;
    
public:
    Capacitor(double c, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    double getCapacity();
    void setTCurrent(double dt);
    std::vector<double> getTCurrent() override;
    std::complex<double> getImpedance(double omega);
};

#endif // CAPACITOR_H