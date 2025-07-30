//
// Created by Purple-Rose on 6/2/2025.
//

#include "CircuitModel.h"

#include <iostream>

#include "../Solver/DCSolverTest.h"
#include"../Solver/Transient.h"
std::vector<std::shared_ptr<Node>> CircuitModel:: getNode() const {return nodes;}
int CircuitModel::getCountOfNodes() const{
    int n = 0;
    for(auto & node : this->getNode()) {
        if(!node->isGround()){
            n++;
        }
    }
    return n;
}
   void CircuitModel::setNodesNumber() const{
        int nodeNumber=0;
        for(auto & node : nodes) {
            if(node->isGround()) {
                node->setNumber(-1);
            }
            else {
                node->setNumber(nodeNumber);
                nodeNumber++;
            }
        }
   }
void CircuitModel::setRefrenceNode(std::string name) {
    for(auto & node : nodes) {
        if(node->getName() == name) {
            node->setAsReference();
        }
    }
}
void CircuitModel::setNotRefrenceNode(std::string name) {
    for(auto & node : nodes) {
        if(node->getName() == name) {
            node->setAsNotReference();
        }
    }
}


 std::vector<std::shared_ptr<Component>>& CircuitModel::getComponents()  {
    return components;
}
int CircuitModel::getCountOfVoltageSources()  {
    int count = 0;
    for (const auto& comp : components) {
        if (typeid(*comp) == typeid(VoltageSource)) {
            count++;
        }
    }
    return count;
}
int CircuitModel::getCountOfInductors()  {
    int count = 0;
    for (const auto& comp : components) {
        if (typeid(*comp) == typeid(Inductor)) {
            count++;
        }
    }
    return count;
}
void CircuitModel::addComponent(std::shared_ptr<Component> comp) {
    components.push_back(comp);
}
std::shared_ptr<Node> CircuitModel::addNode(const std::string& name) {
    for(auto & node : nodes) {
        if(node->getName() == name) {
            return node;
        }
    }
    auto node = std::make_shared<Node>(name);
    nodes.push_back(node);
    return node;
}
void CircuitModel::removeComponent(std::string name) {
    std::string n1;
    std::string n2;
    for(int i=0 ; i<components.size() ; i++) {
        if(components[i]->getName() == name) {
            components.erase(components.begin()+i);
            break;
        }
    }

}



