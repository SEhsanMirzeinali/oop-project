//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#pragma once
#include "..\Circuit Model\CircuitModel.h"
#include<iostream>
#include<iostream>
#include "Diagrom.h"
class SimulationResults {
    private:
    CustomPlotVisualizer plotter;
public:
    std::unordered_map<std::string, double> DC_Analyse_Results( std::vector<double> results, CircuitModel& circuit);
    std::string Transient_Analyse(std::vector<std::vector<double>> results,std::vector<std::string> variables,double TStart,double dt,CircuitModel& circuit) ;
    std::vector<double> AC_Analysis(std::string type,std::vector<std::complex<double>> results , double omega ,double phase, std::vector<std::string> variables,CircuitModel& circuit) ;
    std::vector<std::vector<double>> extractTwoColumns(const std::vector<std::vector<double>>& matrix,std::vector<int>);
    std::vector<double> extractSingleColumn(const std::vector<std::vector<double>>& matrix,int columnIndex);
    void plotResults(
    const std::vector<std::vector<double>>& data,
    const QString& title,
    const QString& xAxisTitle,
    const QString& yAxisTitle,
    const std::vector<std::string>& variables);
    void PlotACAnalysis(
        const std::vector<std::vector<std::complex<double>>>& results,
        const std::vector<std::string>& variables,
        const QString& title = "AC Analysis",
        const QString& xAxisTitle = "Frequency (Hz)",
        const QString& yAxisTitle = "Values");
};



#endif //SIMULATIONRESULTS_H
