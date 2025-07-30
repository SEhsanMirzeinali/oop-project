//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef CIRCUITMODEL_H
#define CIRCUITMODEL_H

#pragma once
#include<memory>
#include<string>
#include<vector>
#include <unordered_map>
#include "Component.h"
#include"components\VoltageSource.h"
#include"components\Resistor.h"
#include"components\CurrentSource.h"
#include"components\Capacitor.h"
#include"components\Inductor.h"



class Node;


class CircuitModel {
private:
    std::vector<std::shared_ptr<Component>> components;

    // std::vector<Resistor*> resistors;
    // std::vector<VoltageSource*> voltageSources;
    // std::vector<Capacitor*> capacitors;
    std::vector<std::shared_ptr<Node>> nodes;
  //  std::unordered_map<std::string, int> nodeNameToIndex;

public:
    // Component management
    void addComponent(std::shared_ptr<Component> comp);
    std::vector<std::shared_ptr<Component>>& getComponents();
    int getCountOfVoltageSources();
    void removeComponent(std::string name);

    // Node management
    std::shared_ptr<Node> addNode(const std::string& name);
    std::vector<std::shared_ptr<Node>> getNode() const;
    int getCountOfNodes() const;
    void setNodesNumber() const;
    void setRefrenceNode(std::string name);
    void setNotRefrenceNode(std::string name);
    int getCountOfInductors();

    // Circuit analysis state
    bool isLinear = true;
    bool needsRetimestamping = true;
};



#endif //CIRCUITMODEL_H
