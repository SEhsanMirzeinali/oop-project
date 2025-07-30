#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "../Component.h"

class Inductor : public Component {
private:
    double inductance;
    std::vector<double> TCurrent;
    
public:
    Inductor(double l, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    std::vector<double> getTCurrent() override;
    double getInductance();
    void setTCurrent(std::vector<double> TCurrent);
};

#endif // INDUCTOR_H