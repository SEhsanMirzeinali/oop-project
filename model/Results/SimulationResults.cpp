//
// Created by Purple-Rose on 6/2/2025.
//

#include "SimulationResults.h"
#include <iomanip>

void SimulationResults::DC_Analyse_Results(std::vector<double> results, CircuitModel& circuit) {
    for (int i = 0; i < results.size(); i++) {
        bool printed = false;
        for (int j = 0; j < circuit.getNode().size(); j++) {
            if (circuit.getNode()[j]->getNumber() == i) {
                std::cout << "node number: " << i << " name: " << circuit.getNode()[j]->getName() << " voltage: ";
                printed = true;
            }
        }
        if (!printed) {
            for (int j = 0; j < circuit.getComponents().size(); j++) {
                std::shared_ptr<Component> comp = circuit.getComponents()[j];
                if (auto vs = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                    std::cout << "voltage source: " << vs->getName() << " current: ";
                }
            }
        }
        std::cout << results[i] << std::endl;
    }
}
std::vector<double> SimulationResults::AC_Analysis(std::string type,std::vector<std::complex<double>> results , double omega ,double phase, std::vector<std::string> variables,CircuitModel& circuit) {
std::vector<std::complex<double>> basicResults;
//circuit.setNodesNumber();
for(int i=0 ; i<results.size()-1 ; i++) {
    for(int j=0 ; j<circuit.getNode().size() ; j++) {
        //std::cout<<circuit.getNode()[j]->getName()<<" num: "<<circuit.getNode()[j]->getNumber()<<std::endl;

        if (circuit.getNode()[j]->getNumber() == i) {
            circuit.getNode()[j]->setCVoltage(results[i]);
        }
        if (circuit.getNode()[j]->getNumber()==-1) {
            circuit.getNode()[j]->setCVoltage({0,0});
        }
    }
}

    for (const auto& comp : circuit.getComponents()) {
        if (auto voltageSource = std::dynamic_pointer_cast<VoltageSource>(comp)) {
            voltageSource->setCCurrent(results[results.size()-1]);
        }
    }

    for (int i = 0; i < variables.size(); i++) {
        //std::cout<<"name: "<<variables[i]<<std::endl;

        if (variables[i][0] == 'V') {
            for (int j = 0; j < circuit.getNode().size(); j++) {
                if (variables[i].substr(1) == circuit.getNode()[j]->getName()) {
                    basicResults.push_back(circuit.getNode()[j]->getCVoltage());
                    break;
                }
            }
        }
        if (variables[i][0] == 'I') {
            for (const auto& comp : circuit.getComponents()) {

                if(variables[i].substr(1) == comp->getName()) {
                    if (auto voltageSource = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                            basicResults.push_back(voltageSource->getCCurrent());
                    }
                    if(auto r = std::dynamic_pointer_cast<Resistor>(comp)) {
                        basicResults.push_back((r->getNode1()->getCVoltage()-r->getNode2()->getCVoltage())/r->getResistance());
                    }
                    if(auto c = std::dynamic_pointer_cast<Capacitor>(comp)) {
                        basicResults.push_back((c->getNode1()->getCVoltage()-c->getNode2()->getCVoltage())/c->getImpedance(omega));
                    }
                    if(auto l = std::dynamic_pointer_cast<Inductor>(comp)) {
                        basicResults.push_back((l->getNode1()->getCVoltage()-l->getNode2()->getCVoltage())/l->getImpedance(omega));
                    }
                }
            }
        }
    }
    // for(int i=0 ; i<basicResults.size() ; i++) {
    //     std::cout<<"basic: "<<basicResults[i]<<" ,";
    // }
    std::cout<<std::endl;
    std::vector<double> finalRes;
    if(type=="AC") {
        finalRes.push_back(omega/6.28);
        for (const auto& complex_num : basicResults) {
            finalRes.push_back(20*std::log10(std::abs(complex_num)));
        }
    }
    else if(type=="Phase") {
        finalRes.push_back(phase);
        for (const auto& complex_num : basicResults) {
            double phase_rad = std::arg(complex_num);
            double phase_deg = phase_rad * 180.0 / 3.14;
            finalRes.push_back(phase_deg);

        }
    }

    double freq = omega/6.28;
    std::cout<<"freq: "<<freq<<std::endl;
    for(int i=0 ; i<variables.size() ; i++) {
        if(variables[i]!="V")
        std::cout<<variables[i]<<"        ";
    }
    std::cout<<std::endl;
    for(int i=0 ; i<finalRes.size() ; i++) {
        std::cout<<finalRes[i]<<", ";
    }
    std::cout<<std::endl;
    return finalRes;
}
    std::vector<std::vector<double>> SimulationResults::Transient_Analyse(std::vector<std::vector<double>> results,
                                        std::vector<std::string> variables,
                                        double TStart,double dt,
                                        CircuitModel& circuit) {
    std::vector<std::vector<double>> finalRes;
    std::vector<int> indexes;

    // Set dt
    //double dt = results[1][0] - results[0][0];
    //double dt=0.1;
    indexes.push_back(0);

    // Set nodes voltage
    for (int i = 0; i < circuit.getNode().size(); i++) {
        for (int j = 1; j < results[0].size(); j++) {
            if (circuit.getNode()[i]->getNumber() == j - 1) {
                circuit.getNode()[i]->setTVoltage(extractSingleColumn(results, j));
            }
        }
        if (circuit.getNode()[i]->isGround()) {
            std::vector<double> zero(results.size(), 0.0);
            circuit.getNode()[i]->setTVoltage(zero);
        }
    }

    // Set voltage sources current
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

    // Set inductors current
    int inductorIndex = 0;
    int inductorColumnStart = currentColumnIndex + voltageSourceCount;
    int inductorCount = circuit.getCountOfInductors();

    for (const auto& comp : circuit.getComponents()) {
        if (auto inductor = std::dynamic_pointer_cast<Inductor>(comp)) {
            if (inductorIndex < inductorCount) {
                inductor->setTCurrent(extractSingleColumn(results, inductorColumnStart + inductorIndex));
                inductorIndex++;
            }
        }
    }

    // Set resistors current
    for (const auto& comp : circuit.getComponents()) {
        if (auto R = std::dynamic_pointer_cast<Resistor>(comp)) {
            R->setTCurrent();
        }
    }

    // Set capacitors current
    for (const auto& comp : circuit.getComponents()) {
        if (auto C = std::dynamic_pointer_cast<Capacitor>(comp)) {
            C->setTCurrent(dt);
        }
    }

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

    std::cout << " Time    ";
    for (int i = 0; i < variables.size(); i++) {
        std::cout << variables[i] << "    ";
    }
    std::cout << std::endl;

    for (int i = TStart / dt + 1; i < finalRes.size(); i++) {
        for (int j = 0; j < finalRes[i].size(); j++) {
            std::cout << std::fixed << std::setprecision(3) << finalRes[i][j] << " | ";
        }
        std::cout << std::endl;
    }
    return finalRes;
}

std::vector<std::vector<double>> SimulationResults::extractTwoColumns(
    const std::vector<std::vector<double>>& matrix,
    std::vector<int> columnIndices
) {
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