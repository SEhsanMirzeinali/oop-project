#ifndef NODE_H
#define NODE_H

#include <complex>
#include <vector>
#include <string>

class Node {
private:
    int number;
    std::string name;
    double voltage = 0.0;  // Default voltage (reference node is 0)
    std::vector<double> TVoltage;
    std::complex<double> CVoltage;
    bool isReference = false;

public:
    Node(const std::string& name);
    
    // Getters
    int getNumber() const;
    std::string getName() const;
    double getVoltage() const;
    bool isGround() const;
    std::vector<double> getTVoltage() const;
    std::complex<double> getCVoltage();

    // Setters
    void setVoltage(double v);
    void setAsReference();
    void setAsNotReference();
    void setNumber(int num);
    void setName(const std::string& name);
    void setTVoltage(const std::vector<double>& v);
    void setCVoltage(const std::complex<double>& v);

    // Helper for netlist generation
    std::string toNetlist() const;
};

#endif // NODE_H