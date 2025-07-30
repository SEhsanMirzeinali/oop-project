#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "../Component.h"

class VoltageSource : public Component {
private:
    double voltage;
    double current;
    std::vector<double> TCurrent;
    
public:
    VoltageSource(double v, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    std::vector<double> getTCurrent() override;
    void setTCurrent(std::vector<double> c);
};

#endif // VOLTAGESOURCE_H