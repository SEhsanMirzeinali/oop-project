//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#pragma once
#include "..\Circuit Model\CircuitModel.h"
<<<<<<< HEAD
=======
#include "SimulationResults.h"
#include <QtCharts/QValueAxis>  // برای QValueAxis
#include <QtWidgets/QMainWindow> // برای QMainWindow
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "Diagrom.h"
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
#include<iostream>

class SimulationResults {
    private:
<<<<<<< HEAD
=======
    CustomPlotVisualizer plotter;
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
public:
    void DC_Analyse_Results(std::vector <double> results,CircuitModel& circuit);
    void Transient_Analyse(std::vector <std::vector<double>> results,std::vector<std::string> variables,double,CircuitModel& circuit);
    std::vector<std::vector<double>> extractTwoColumns(const std::vector<std::vector<double>>& matrix,std::vector<int>);
    std::vector<double> extractSingleColumn(const std::vector<std::vector<double>>& matrix,int columnIndex);
<<<<<<< HEAD
=======
    void plotResults(
        const std::vector<std::vector<double>>& data,
        const QString& title,
        const QString& yAxisTitle,
        const std::vector<std::string>& variables
    );
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
};



#endif //SIMULATIONRESULTS_H
