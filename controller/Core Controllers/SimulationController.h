//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#pragma once
#include "../model/solvers/DCSolver.h"
#include "../view/CommandHandler.h"
class CircuitModel;
class SimulationController {
private:
    CircuitModel& circuit;
    Transient transientSolver;

public:
    SimulationController(CircuitModel& circuit) : circuit(circuit) {}



    // Run Transient Analysis
    SimulationResults runTransientAnalysis(double timeStep, double duration) {
        return transientSolver.solve(circuit, timeStep, duration);
    }



    // Stop ongoing simulation (if multi-threaded)
    //void stopSimulation() { ... }
};



#endif //SIMULATIONCONTROLLER_H
