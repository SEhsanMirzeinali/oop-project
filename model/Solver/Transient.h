// //
// // Created by Purple-Rose on 6/2/2025.
// //
//
 #ifndef TRANSIENT_H
 #define TRANSIENT_H
//
#pragma once
 #include "../Results/SimulationResults.h"
#include "..\Circuit Model\CircuitModel.h"
//
 class Transient {
    std::shared_ptr<SimulationResults> circuitResults ;
public:
    Transient() : circuitResults(std::make_shared<SimulationResults>()){}
    std::vector<std::vector<double>> solve(CircuitModel& circuit,double dt,double t_end,double,double,std::vector<std::string>);
    std::vector<std::vector<double>> createMatG( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatB( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatC(CircuitModel& circuit);
    std::vector<std::vector<double>> createMatD( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatDynamic(CircuitModel& circuit);
    std::vector<double> createMatE( CircuitModel& circuit);
    std::vector<double> createMatJ( CircuitModel& circuit);
    std::vector<double> combineRightSide(const std::vector<double>& J, const std::vector<double>& E);
    std::vector<std::vector<double>> combineLeftSide(
    const std::vector<std::vector<double>>& G,const std::vector<std::vector<double>>& B,
    const std::vector<std::vector<double>>& C,const std::vector<std::vector<double>>& D);
    std::vector<std::vector<double>> addTimeColumn(const std::vector<std::vector<double>>& solution, double dt);
    std::vector<std::vector<double>> solveMatrixODE(const std::vector<std::vector<double>> &G, const std::vector<std::vector<double>> &C, const std::vector<double> &F, const
                                                    std::vector<double> &X0, double dt, double t_end) ;
    void printvector(const std::vector<std::vector<double>>& vec);

 };
//
//
//
 #endif //TRANSIENT_H
