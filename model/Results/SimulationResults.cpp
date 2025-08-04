//
// Created by Purple-Rose on 6/2/2025.
//

#include "SimulationResults.h"
<<<<<<< HEAD

=======
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
#include <iomanip>

void SimulationResults:: DC_Analyse_Results(std::vector <double> results,CircuitModel& circuit){
    for(int i=0;i<results.size();i++){
        bool printed = false;
        for(int j=0 ; j<circuit.getNode().size();j++) {
            if(circuit.getNode()[j]->getNumber()==i) {
                std::cout <<"node number: "<<i <<" name: "<<circuit.getNode()[j]->getName()<<" voltage: ";
                printed = true;
            }
        }
        if(!printed) {
            for(int j=0 ; j<circuit.getComponents().size();j++) {
                std::shared_ptr<Component> comp = circuit.getComponents()[j];
                if (auto vs = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                    std::cout <<"voltage source: "<<vs->getName()<<" current: ";
                }
            }
        }
        std::cout << results[i] << std::endl;
    }
}
<<<<<<< HEAD
=======
void SimulationResults::plotResults(
    const std::vector<std::vector<double>>& data,
    const QString& title,
    const QString& yAxisTitle,
    const std::vector<std::string>& variables
) {
    std::vector<std::vector<std::vector<double>>> multiData;
    std::vector<QString> legendNames;

    // استخراج ستون‌های مورد نظر (ستون ۰ = زمان، ستون‌های بعدی = متغیرها)
    for (size_t i = 1; i < data[0].size(); ++i) {
        std::vector<std::vector<double>> singleVarData;
        for (const auto& row : data) {
            singleVarData.push_back({row[0], row[i]}); // زمان و مقدار متغیر
        }
        multiData.push_back(singleVarData);
        legendNames.push_back(QString::fromStdString(variables[i - 1]));
    }

    plotter.plotBasicGraph(multiData, title, "زمان (ثانیه)", yAxisTitle, legendNames);
}
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
void SimulationResults::Transient_Analyse(std::vector<std::vector<double>> results, std::vector<std::string> variables,double TStart, CircuitModel &circuit) {
std::vector <std::vector<double>> finalRes;
std::vector <int> indexes;
    //set dt
    double dt=results[5][0]-results[4][0];

    indexes.push_back(0);
    //set nodees voltage
for(int i=0 ; i<circuit.getNode().size();i++) {
    for(int j=1 ; j<results[0].size() ; j++) {
        if(circuit.getNode()[i]->getNumber()==j-1) {
            circuit.getNode()[i]->setTVoltage(extractSingleColumn(results,j));
;        }
    }
    if(circuit.getNode()[i]->isGround()) {
        std::vector<double> zero(results.size(), 0.0);
        circuit.getNode()[i]->setTVoltage(zero);
    }
}

    //set voltage sources current

    int nodeCount = circuit.getCountOfNodes();
    int voltageSourceCount = circuit.getCountOfVoltageSources();
    int currentColumnIndex = nodeCount + 1;

    int vsIndex = 0;
    for (const auto& comp : circuit.getComponents()) {
        if (auto voltageSource = std::dynamic_pointer_cast<VoltageSource>(comp)) {
            if (vsIndex < voltageSourceCount) {
                voltageSource->setTCurrent(extractSingleColumn(results, currentColumnIndex + vsIndex));
                vsIndex++;
            }
        }
    }

    //set inductors current
    int inductorIndex = 0;
    int inductorColumnStart = currentColumnIndex + voltageSourceCount;
    int inductorCount=circuit.getCountOfInductors();

    for (const auto& comp : circuit.getComponents()) {
        if (auto inductor = std::dynamic_pointer_cast<Inductor>(comp)) {
            if (inductorIndex < inductorCount) {
                inductor->setTCurrent(extractSingleColumn(results, inductorColumnStart + inductorIndex));
                inductorIndex++;
            }
        }
    }
    //set resistors current
    for (const auto& comp : circuit.getComponents()) {
        if (auto R = std::dynamic_pointer_cast<Resistor>(comp)) {
            R->setTCurrent();
        }
    }
    //set capacitors current
    for (const auto& comp : circuit.getComponents()) {
        if (auto C = std::dynamic_pointer_cast<Capacitor>(comp)) {
            C->setTCurrent(dt);
        }
    }

    // for (const auto& comp : circuit.getComponents()) {
    //     if (auto I = std::dynamic_pointer_cast<VoltageSource>(comp)) {
    //         std::cout<<"current: "<<I->getName()<<"\n";
    //     }
    // }


    std::vector<double> firstCol = extractSingleColumn(results, 0);
    finalRes.resize(firstCol.size());

    for (size_t i = 0; i < firstCol.size(); ++i) {
        finalRes[i].push_back(firstCol[i]);
    }



    for (int i = 0; i < variables.size(); i++) {
        if (variables[i][0] == 'V') {
            for (int j = 0; j < circuit.getNode().size(); j++) {
                if (variables[i].substr(1) == circuit.getNode()[j]->getName()) {
                    std::vector<double> voltages = circuit.getNode()[j]->getTVoltage();
                    for (size_t k = 0; k < finalRes.size(); ++k) {
                        finalRes[k].push_back(voltages[k]);
                    }
                }
            }
        }
        if (variables[i][0] == 'I') {
            for (int j = 0; j < circuit.getComponents().size(); j++) {
                if (variables[i].substr(1) == circuit.getComponents()[j]->getName()) {
                    std::vector<double> currents = circuit.getComponents()[j]->getTCurrent();
                    for (size_t k = 0; k < finalRes.size(); ++k) {
                        finalRes[k].push_back(currents[k]);
                    }
                }
            }
        }

    }
    std::cout<<" Time    ";
    for(int i=0 ; i<variables.size(); i++) {
        std::cout<<variables[i]<<"    ";
    }
    std::cout<<std::endl;
    for(int i=TStart/dt+1;i<finalRes.size();i++) {
        for(int j=0 ; j<finalRes[i].size();j++) {
            std::cout<<std::fixed<<std::setprecision(3)<<finalRes[i][j]<<" | ";
        }
        std::cout<<std::endl;
    }
<<<<<<< HEAD
=======

    if (!variables.empty()) {
        QString title = "Transient Analysis";
        QString yAxisTitle = "مقادیر";
        plotResults(finalRes, title, yAxisTitle, variables); // ارسال variables برای نام‌گذاری نمودارها
    }
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
}

std::vector<std::vector<double>> SimulationResults:: extractTwoColumns(const std::vector<std::vector<double>>& matrix,std::vector<int> columnIndices) {

        std::vector<std::vector<double>> result;


        for (const auto& row : matrix) {
            std::vector<double> selectedColumns;
            for (int col : columnIndices) {
                if (col >= 0 && col < row.size()) {
                    selectedColumns.push_back(row[col]);
                }
            }
            result.push_back(selectedColumns);
        }

        return result;

}
#include <vector>

std::vector<double> SimulationResults::extractSingleColumn(
    const std::vector<std::vector<double>>& matrix,
    int columnIndex
) {
    std::vector<double> result;

    for (const auto& row : matrix) {
        if (columnIndex >= 0 && columnIndex < row.size()) {
            result.push_back(row[columnIndex]);
        }
    }

    return result;
}

