//
// Created by Purple-Rose on 8/4/2025.
//

#ifndef ACSWEEP_H
#define ACSWEEP_H


#pragma once
 #include "../Results/SimulationResults.h"
#include "..\Circuit Model\CircuitModel.h"
#include<complex>
class ACSweep {
    std::shared_ptr<SimulationResults> circuitResults ;
public:
    ACSweep() : circuitResults(std::make_shared<SimulationResults>()){}
    std::vector<std::vector<std::complex<double>>> solve(CircuitModel& circuit,double startFreq , double endFreq ,
        int numOfPoints ,std::string typeOfSweep,std::vector<std::string> variables);
    std::vector<std::vector<std::complex<double>>> Phasesolve(CircuitModel& circuit,double baseFreq,double StartPhase ,double EndPhase , int numOfPoints ,std::vector<std::string> variables);
    std::vector<std::vector<std::complex<double>>> createMatG(CircuitModel& circuit);
    std::vector<std::vector<std::complex<double>>> createMatB( CircuitModel& circuit);
    std::vector<std::vector<std::complex<double>>>createMatC(CircuitModel& circuit);
    std::vector<std::vector<std::complex<double>>> createMatD( CircuitModel& circuit);
    std::vector<std::vector<double>> createMatDynamic(CircuitModel& circuit);
    std::vector<std::complex<double>> createMatE(CircuitModel& circuit);
    std::vector<std::complex<double>> createMatJ( CircuitModel& circuit);
    std::vector<std::complex<double>> combineRightSide( const std::vector<std::complex<double>>& J,  // n × 1
    const std::vector<std::complex<double>>& E);
    std::vector<std::vector<std::complex<double>>> combineLeftSide(
    const std::vector<std::vector<std::complex<double>>>& G,
    const std::vector<std::vector<std::complex<double>>>& B,
    const std::vector<std::vector<std::complex<double>>>& C,
    const std::vector<std::vector<std::complex<double>>>& D);
    std::vector<std::vector<double>> addTimeColumn(const std::vector<std::vector<double>>& solution, double dt);
    std::vector<std::complex<double>> solveComplexSystem(const std::vector<std::vector<std::complex<double>>>& A,
                                             const std::vector<std::complex<double>>& b);
    void printvector(const std::vector<std::vector<double>>& vec);
};



#endif //ACSWEEP_H
