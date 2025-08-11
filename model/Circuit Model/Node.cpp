#include "Node.h"

Node::Node(const std::string& name) : name(name) {}

// Getters implementation
int Node::getNumber() const {
    return number;
}

std::string Node::getName() const {
    return name;
}

double Node::getVoltage() const {
    return voltage;
}

bool Node::isGround() const {
    return isReference;
}

std::vector<double> Node::getTVoltage() const {
    return TVoltage;
}

std::complex<double> Node::getCVoltage() {
    return CVoltage;
}

// Setters implementation
void Node::setVoltage(double v) {
    voltage = v;
}

void Node::setAsReference() {
    isReference = true;
    voltage = 0.0;
}

void Node::setAsNotReference() {
    isReference = false;
}

void Node::setNumber(int num) {
    number = num;
}

void Node::setName(const std::string& name) {
    this->name = name;
}

void Node::setTVoltage(const std::vector<double>& v) {
    TVoltage = v;
}
void Node::setCVoltage(const std::complex<double> &v) {
    CVoltage = v;
}

// Helper method implementation
std::string Node::toNetlist() const {
    return isReference ? "0" : name;
}