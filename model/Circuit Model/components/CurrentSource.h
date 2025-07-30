#ifndef CURRENTSOURCE_H
#define CURRENTSOURCE_H

#include "../Component.h"

class CurrentSource : public Component {
private:
    double current;
    
public:
    CurrentSource(double i, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    std::vector<double> getTCurrent() override;
};

#endif // CURRENTSOURCE_H