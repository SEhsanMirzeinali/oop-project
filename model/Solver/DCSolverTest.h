//
// Created by Purple-Rose on 6/3/2025.
//

#ifndef DCSOLVERTEST_H
#define DCSOLVERTEST_H

#include "../Results/SimulationResults.h"
#include "..\Circuit Model\CircuitModel.h"

class DCSolverTest {
    std::shared_ptr<SimulationResults> circuitResults ;
  public:
    DCSolverTest() : circuitResults(std::make_shared<SimulationResults>()){}
    //CircuitModel testCircuit;
    void initializeTestCircuit();
    std::vector<double> solve(CircuitModel& circuit);
    std::vector<std::vector<double>> createMatG( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatB( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatC(CircuitModel& circuit);
    std::vector<std::vector<double>> createMatD( CircuitModel& circuit);
    std::vector<double> createMatE( CircuitModel& circuit);
    std::vector<double> createMatJ( CircuitModel& circuit);
    std::vector<double> combineRightSide(const std::vector<double>& J, const std::vector<double>& E);
    std::vector<std::vector<double>> combineLeftSide(
    const std::vector<std::vector<double>>& G,const std::vector<std::vector<double>>& B,
    const std::vector<std::vector<double>>& C,const std::vector<std::vector<double>>& D);
    std::vector<double>solveLinearSystem(std::vector<std::vector<double>> A, std::vector<double> b);

    void printvector(const std::vector<std::vector<double>>& vec);


};



#endif //DCSOLVERTEST_H
