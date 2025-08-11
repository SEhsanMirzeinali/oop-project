#ifndef RESISTOR_H
#define RESISTOR_H

#include "../Component.h"

class Resistor : public Component {
private:
    double resistance;
    std::vector<double> tCurrent;
    
public:
    Resistor(double r, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    double getConductance();
    double getResistance();
    void setTCurrent();
    std::vector<double> getTCurrent() override;
};

#endif // RESISTOR_H