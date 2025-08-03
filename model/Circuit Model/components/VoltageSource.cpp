#include "VoltageSource.h"

#include <complex>
#include <cmath>

#include <iostream>

VoltageSource::VoltageSource(std::string t, const std::string& name, Node* n1, Node* n2)
    : Component(name, n1, n2), type(t) {}

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
double VoltageSource::sinFunction(double time , double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles) {
    double TTotal = TDelay + NCycles / freq;
    if(time < TDelay || (NCycles>0 && time > TTotal)) {
        return offset;
    }
    //return sin(2.0*3.1415*freq*time);
    return (ampl * exp(-theta*(time-TDelay)) * sin(2.0*3.14*freq*(time - TDelay) + phi)) + offset;
}
double VoltageSource::pulseFnction(double time, double VInitial, double Von,double TDelay, double TRise,double TFall ,double TOn, double TPeriod,double NCycles) {
    double Ttotal = NCycles * TPeriod;
    time = fmod(time, TPeriod);

    if (time < TDelay)
        return VInitial;
    else if (time < TDelay + TRise)
        return VInitial + (Von - VInitial) * (time - TDelay) / TRise;
    else if (time < TDelay + TRise + TOn)
        return Von;
    else if (time < TDelay + TRise + TOn + TFall)
        return Von - (Von - VInitial) * (time - TDelay - TRise - TOn) / TFall;
    else
        return VInitial;

}
void VoltageSource:: setSinVariables(double offset , double ampl , double freq , double TDelay, double theta, double phi, double NCycles) {
    this->offset = offset;
    this->ampl = ampl;
    this->freq = freq;
    this->TDelay = TDelay;
    this->theta = theta;
    this->phi = phi;
    this->NCycles = NCycles;
}
//add type of source
void VoltageSource::setVoltage() {
    if (type == "DC") {
        voltage = DCVoltage;
    }
    else if(type == "SINE"){
        voltage = sinFunction(time,offset,ampl,freq,TDelay,theta,phi,NCycles);
    }
    else if(type == "PULSE") {
        voltage = pulseFnction(time,VInitial,Von,TDelay,TRise,TFall,TOn,TPeriod,NCycles);
    }
}
void VoltageSource:: setPulseVariables(double VInitial, double Von,double TDelay, double TRise,double TFall ,double TOn, double TPeriod,double NCycles) {
    this->VInitial = VInitial;
    this->Von = Von;
    this->TDelay = TDelay;
    this->TRise = TRise;
    this->TFall = TFall;
    this->TOn = TOn;
    this->TPeriod = TPeriod;
    this->NCycles = NCycles;
}

void VoltageSource::setTime(double t) {time = t;}
void VoltageSource::setDcVariables(double DCVoltage) {this->DCVoltage = DCVoltage;}
