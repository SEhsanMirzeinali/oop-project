#ifndef AC_H
#define AC_H

#pragma once
#include "../Results/SimulationResults.h"
#include "..\Circuit Model\CircuitModel.h"
#include <complex>
#include <vector>

class AC {
    std::shared_ptr<SimulationResults> circuitResults;
public:
    AC() : circuitResults(std::make_shared<SimulationResults>()) {}
    std::vector<std::vector<std::complex<double>>> solve(
        CircuitModel& circuit,
        double startFreq,
        double endFreq,
        int points,
        std::vector<std::string> variables
    );
    std::vector<std::vector<std::complex<double>>> createMatG(CircuitModel& circuit, double omega);
    std::vector<std::vector<std::complex<double>>> createMatB(CircuitModel& circuit);
    std::vector<std::vector<std::complex<double>>> createMatC(CircuitModel& circuit);
    std::vector<std::vector<std::complex<double>>> createMatD();
    std::vector<std::complex<double>> createMatE(CircuitModel& circuit, double omega);
    std::vector<std::complex<double>> createMatJ(CircuitModel& circuit, double omega);
    std::vector<std::vector<std::complex<double>>> combineLeftSide(
        const std::vector<std::vector<std::complex<double>>>& G,
        const std::vector<std::vector<std::complex<double>>>& B,
        const std::vector<std::vector<std::complex<double>>>& C,
        const std::vector<std::vector<std::complex<double>>>& D
    );
    std::vector<std::complex<double>> combineRightSide(
        const std::vector<std::complex<double>>& J,
        const std::vector<std::complex<double>>& E
    );
    void printComplexVector(const std::vector<std::vector<std::complex<double>>>& vec);
};

#endif // AC_H