//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#pragma once
#include "..\Circuit Model\CircuitModel.h"
#include<iostream>

class SimulationResults {
    private:
public:
    void DC_Analyse_Results(std::vector <double> results,CircuitModel& circuit);
    void Transient_Analyse(std::vector <std::vector<double>> results,std::vector<std::string> variables,double,double dt,CircuitModel& circuit);
    std::vector<std::vector<double>> extractTwoColumns(const std::vector<std::vector<double>>& matrix,std::vector<int>);
    std::vector<double> extractSingleColumn(const std::vector<std::vector<double>>& matrix,int columnIndex);
};



#endif //SIMULATIONRESULTS_H
