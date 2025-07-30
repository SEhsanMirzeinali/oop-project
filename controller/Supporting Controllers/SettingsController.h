//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H



class SettingsController {
private:
    SolverConfig& config;

public:
    SettingsController(SolverConfig& config) : config(config) {}

    // Set time step for transient analysis
    void setTimeStep(double step) {
        config.timeStep = step;
    }

    // Choose solver type (e.g., LU decomposition, Gauss-Seidel)
    void setSolverType(SolverType type) {
        config.solverType = type;
    }
};



#endif //SETTINGSCONTROLLER_H
