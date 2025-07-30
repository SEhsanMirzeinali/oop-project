//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef RESULTCONTROLLER_H
#define RESULTCONTROLLER_H


#include "..\..\model\Circuit Model\CircuitModel.h"

class ResultController {
private:
    SimulationResults results;

public:
  void handle_DC_Analyse(std::vector <double> results,CircuitModel& circuit);
    // Save results to a file
//    void saveResults(const string& filePath, ExportFormat format) {
//        switch (format) {
//            case CSV: saveAsCSV(filePath); break;
//            case JSON: saveAsJSON(filePath); break;
//        }
//    }
//
//    // Load results from a file
//    void loadResults(const string& filePath) {
//        results = loadFromFile(filePath);
//    }
//
//    // Get results for plotting
//    const SimulationResults& getResults() const { return results; }
};



#endif //RESULTCONTROLLER_H
