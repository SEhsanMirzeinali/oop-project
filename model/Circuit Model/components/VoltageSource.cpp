#include "VoltageSource.h"

#include <complex>
#include <cmath>

#include <iostream>

VoltageSource::VoltageSource(double v, const std::string& name, Node* n1, Node* n2) 
    : Component(name, n1, n2), voltage(v) {}

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
void VoltageSource::setVoltage(double t) {
    voltage=sinFunction(time,offset,ampl,freq,TDelay,theta,phi,NCycles);
}
void VoltageSource::setTime(double t) {time = t;}
