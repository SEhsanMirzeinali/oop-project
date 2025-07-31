#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "../Component.h"

class VoltageSource : public Component {
private:
    double voltage;
    double current;
    std::vector<double> TCurrent;
    double time;
    double dt;
    double offset;
    double ampl;
    double freq;
    double TDelay;
    double theta;
    double phi;
    double NCycles;

public:
    VoltageSource(double v, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    double getCurrent() override;
    std::vector<double> getTCurrent() override;
    void setTCurrent(std::vector<double> c);
    double sinFunction(double time , double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles);
    void setSinVariables(double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles);
    void setVoltage(double v);
    void setTime(double t);
    double getTime(){return time;}
};

#endif // VOLTAGESOURCE_H