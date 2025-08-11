#ifndef VOLTAGESOURCE_H
#define VOLTAGESOURCE_H

#include "../Component.h"

class VoltageSource : public Component {
private:
    double voltage;
    double current;
    std::vector<double> TCurrent;
    double time;
    double dt, offset, ampl, freq, TDelay, theta, phi;
    double VInitial, Von, TRise, TOn, TFall, TPeriod;
    double NCycles;
    double DCVoltage;
    std::string type;
    double omega;
    double phase;
    std::complex<double> complexVoltage;
    std::complex<double> complexCurrent;

public:
    VoltageSource(std::string t, const std::string& name, Node* n1, Node* n2);
    
    std::string getName() override;
    Node* getNode1() override;
    Node* getNode2() override;
    double getVoltage() override;
    std::complex<double> getComplexVoltage();
    double getCurrent() override;
    std::vector<double> getTCurrent() override;
    void setCCurrent(const std::complex<double>& v);
    std::complex<double> getCCurrent();

    void setTCurrent(std::vector<double> c);
    double sinFunction(double time , double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles);
    void setSinVariables(double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles);
    void setDcVariables(double DCVoltage);
    void setPulseVariables(double VInitial, double Von,double TDelay, double TRise,double TFall ,double TOn, double TPeriod,double NCycles);
    double pulseFunction(double time, double VInitial, double Von,double TDelay, double TRise,double TFall ,double TOn, double TPeriod,double NCycles);
    void setACVariables(double ampl , double phase);
    void setVoltage();
    void setComplexVoltage(std::complex<double> CV);
    void setTime(double t);
    void setOmega(double o);
    void setType(std::string t);
    void setPhase(double p);
    double getTime();
    double getOmega();
    double getPhase();

};

#endif // VOLTAGESOURCE_H