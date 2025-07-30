//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef NODE_H
#define NODE_H

#pragma once

#include<string>
class Node {
private:
    int number;
    std::string name;
    double voltage = 0.0;  // Default voltage (reference node is 0)
    std::vector<double> TVoltage;
    bool isReference = false;

public:
       Node (const std:: string &name): name(name) {};


    // Getters
    int getNumber() const { return number; }
    std::string getName() const { return name; }
    double getVoltage() const { return voltage; }
    bool isGround() const { return isReference; }
    std::vector<double> getTVoltage() const { return TVoltage; }

    // Setters
    void setVoltage(double v) { voltage = v; }
    void setAsReference() { isReference = true; voltage = 0.0;; };
    void setAsNotReference() { isReference = false; }
    void setNumber(const int num){this->number = num;}
    void setName(const std::string name){this->name = name;}
    void setTVoltage(std::vector<double> v){TVoltage=v;}

    //موقع تحلیل مدار اینجوری بهشون شماره میدم که بذارم توی ماتریس
//    int nodeNumber=0;
//    for(auto & node : nodes) {
//        if(node->getGround()) {
//            node->setNumber(-1);
//        }
//        else {
//            node->setNumber(nodeNumber);
//            nodeNumber++;
//        }
//    }

    // Helper for netlist generation
    std::string toNetlist() const {
        return isReference ? "0" : name;
    }
};



#endif //NODE_H
