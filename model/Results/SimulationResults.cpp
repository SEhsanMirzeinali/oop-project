//
// Created by Purple-Rose on 6/2/2025.
//

#include "SimulationResults.h"
#include <iomanip>

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDateTime>

std::unordered_map<std::string, double> SimulationResults::DC_Analyse_Results(std::vector<double> results,  CircuitModel& circuit)
{
    std::unordered_map<std::string, double> result_map;

    for (int i = 0; i < results.size(); i++) {
        bool is_node = false;

        // Check if this index corresponds to a node
        for (const auto& node : circuit.getNode()) {
            if (node->getNumber() == i) {
                result_map[node->getName()] = results[i];
                is_node = true;
                break;
            }
        }

        // If not a node, check if it's a voltage source current
        if (!is_node) {
            for (const auto& comp : circuit.getComponents()) {
                if (auto vs = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                    // For voltage sources, we'll use a special key format
                    result_map["I("+ vs->getName() +")"] = results[i];
                }
            }
        }
    }

    return result_map;
}
std::vector<double> SimulationResults::AC_Analysis(
    std::string type,
    std::vector<std::complex<double>> results,
    double omega,
    double phase,
    std::vector<std::string> variables,
    CircuitModel& circuit)
{
    std::vector<std::complex<double>> basicResults;

    // 1. Set complex voltages for nodes
    for(int i = 0; i < results.size()-1; i++) {
        for(const auto& node : circuit.getNode()) {
            if(node->getNumber() == i) {
                node->setCVoltage(results[i]);
            }
            if(node->getNumber() == -1) {
                node->setCVoltage({0,0});
            }
        }
    }

    // 2. Set complex current for voltage sources
    for(const auto& comp : circuit.getComponents()) {
        if(auto voltageSource = std::dynamic_pointer_cast<VoltageSource>(comp)) {
            voltageSource->setCCurrent(results.back()); // Last element is current
        }
    }

    // 3. Process requested variables
    for(const auto& var : variables) {
        if(var.empty()) continue;

        try {
            if(var[0] == 'V') {
                // Handle voltage variables
                std::string nodeName = var.substr(1);
                bool found = false;

                for(const auto& node : circuit.getNode()) {
                    if(node->getName() == nodeName) {
                        basicResults.push_back(node->getCVoltage());
                        found = true;
                        break;
                    }
                }

                if(!found) {
                    std::cerr << "Warning: Node " << nodeName << " not found for variable " << var << std::endl;
                    basicResults.push_back({0,0});
                }
            }
            else if(var[0] == 'I') {
                // Handle current variables
                std::string compName = var.substr(1);
                bool found = false;

                for(const auto& comp : circuit.getComponents()) {
                    if(comp->getName() == compName) {
                        if(auto vs = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                            basicResults.push_back(vs->getCCurrent());
                        }
                        else if(auto r = std::dynamic_pointer_cast<Resistor>(comp)) {
                            auto v_diff = r->getNode1()->getCVoltage() - r->getNode2()->getCVoltage();
                            basicResults.push_back(v_diff / r->getResistance());
                        }
                        else if(auto c = std::dynamic_pointer_cast<Capacitor>(comp)) {
                            auto v_diff = c->getNode1()->getCVoltage() - c->getNode2()->getCVoltage();
                            basicResults.push_back(v_diff / c->getImpedance(omega));
                        }
                        else if(auto l = std::dynamic_pointer_cast<Inductor>(comp)) {
                            auto v_diff = l->getNode1()->getCVoltage() - l->getNode2()->getCVoltage();
                            basicResults.push_back(v_diff / l->getImpedance(omega));
                        }
                        found = true;
                        break;
                    }
                }

                if(!found) {
                    std::cerr << "Warning: Component " << compName << " not found for variable " << var << std::endl;
                    basicResults.push_back({0,0});
                }
            }
            else {
                std::cerr << "Warning: Invalid variable prefix in " << var << " (should be V or I)" << std::endl;
                basicResults.push_back({0,0});
            }
        }
        catch(const std::exception& e) {
            std::cerr << "Error processing variable " << var << ": " << e.what() << std::endl;
            basicResults.push_back({0,0});
        }
    }

    // 4. Prepare final results based on analysis type
    std::vector<double> finalRes;
    double freq = omega / (2 * M_PI);  // More precise than 6.28

    if(type == "AC") {
        finalRes.push_back(freq);
        for(const auto& complex_num : basicResults) {
            double magnitude = std::abs(complex_num);
            // Handle very small magnitudes to avoid log10(0)
            //std::cout << "complex_num:  " << complex_num << std::endl;
            finalRes.push_back(magnitude > 1e-20 ? 20*log10(magnitude) : -200);
        }
    }
    else if(type == "Phase") {
        finalRes.push_back(phase);
        for(const auto& complex_num : basicResults) {
            double phase_rad = std::arg(complex_num);
            double phase_deg = phase_rad * 180.0 / M_PI;  // More precise than 3.14
            finalRes.push_back(phase_deg);
        }
    }
    else {
        std::cerr << "Error: Unknown analysis type " << type << std::endl;
        return {};
    }

    // 5. Print results (optional, can be removed or made configurable)
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Frequency: " << freq << " Hz" << std::endl;
    std::cout << "Variables: ";
    for(const auto& var : variables) {
        std::cout << std::setw(12) << var;
    }
    std::cout << std::endl << "Values:    ";
    for(size_t i = 1; i < finalRes.size(); i++) {
        std::cout << std::setw(12) << finalRes[i];
    }
    std::cout << std::endl << std::endl;

    return finalRes;
}
std::string SimulationResults::Transient_Analyse(std::vector<std::vector<double>> results,std::vector<std::string> variables,double TStart,double dt,CircuitModel& circuit) {
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

    // ===== ذخیره‌سازی نتایج به صورت JSON =====
    QJsonObject root;
    root["title"] = "Transient Analysis";
    root["time_start"] = TStart;
    root["time_step"] = dt;

    // ذخیره هدرها
    QJsonArray headersArray;
    headersArray.append("Time");
    for (const auto& var : variables) {
        headersArray.append(QString::fromStdString(var));
    }
    root["headers"] = headersArray;

    // ذخیره داده‌ها
    QJsonArray dataArray;
    for (int i = TStart / dt + 1; i < finalRes.size(); ++i) {
        QJsonArray rowArray;
        for (double val : finalRes[i]) {
            rowArray.append(val);
        }
        dataArray.append(rowArray);
    }
    root["data"] = dataArray;

    // ساختن داکیومنت JSON
    QJsonDocument doc(root);

    // مسیر ذخیره
    QString fileName = QString("Analysis_%1.json")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QCoreApplication::applicationDirPath() + "/" + fileName;

    // ذخیره در فایل
    QFile jsonFile(filePath);
    if (jsonFile.open(QIODevice::WriteOnly)) {
        jsonFile.write(doc.toJson(QJsonDocument::Indented));
        jsonFile.close();
        qDebug() << "JSON saved at:" << filePath;
    } else {
        qDebug() << "Error saving JSON file:" << jsonFile.errorString();
    }


    ////////
    if (!variables.empty()) {
        QString title = "Transient Analysis";
        QString xAxisTitle = "Time (s)";  // عنوان محور X برای تحلیل Transient
        QString yAxisTitle = "Values";
        plotResults(finalRes, title, xAxisTitle, yAxisTitle, variables);
    }

    //return filename; // اسم فایل رو برمی‌گردونیم برای خواندن بعدی
    return "s";
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

// std::vector<std::vector<double>> SimulationResults::extractTwoColumns(
//     const std::vector<std::vector<double>>& matrix,
//     std::vector<int> columnIndices
// ) {
//     std::vector<std::vector<double>> result;
//
//     for (const auto& row : matrix) {
//         std::vector<double> selectedColumns;
//         for (int col : columnIndices) {
//             if (col >= 0 && col < row.size()) {
//                 selectedColumns.push_back(row[col]);
//             }
//         }
//         result.push_back(selectedColumns);
//     }
//
//     return result;
// }

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
void SimulationResults::plotResults(
    const std::vector<std::vector<double>>& data,
    const QString& title,
    const QString& xAxisTitle,  // پارامتر جدید اضافه شده
    const QString& yAxisTitle,
    const std::vector<std::string>& variables
) {
    std::vector<std::vector<std::vector<double>>> multiData;
    std::vector<QString> legendNames;

    // استخراج ستون‌های مورد نظر (ستون ۰ = زمان/فرکانس، ستون‌های بعدی = متغیرها)
    for (size_t i = 1; i < data[0].size(); ++i) {
        std::vector<std::vector<double>> singleVarData;
        for (const auto& row : data) {
            singleVarData.push_back({row[0], row[i]}); // زمان/فرکانس و مقدار متغیر
        }
        multiData.push_back(singleVarData);
        legendNames.push_back(QString::fromStdString(variables[i - 1]));
    }

    plotter.plotBasicGraphWithMathOps(multiData, title, xAxisTitle, yAxisTitle, legendNames , false);
}

void SimulationResults::PlotACAnalysis(
    const std::vector<std::vector<std::complex<double>>>& results,
    const std::vector<std::string>& variables,
    const QString& title,
    const QString& xAxisTitle,
    const QString& yAxisTitle)
{
    if (results.empty() || variables.empty()) return;

    // جدا کردن داده‌های دامنه و فاز
    std::vector<std::vector<std::vector<double>>> magnitudeData;
    std::vector<std::vector<std::vector<double>>> phaseData;
    std::vector<QString> legendNames;

    // ستون اول: فرکانس
    // ستون‌های بعدی: متغیرها
    for (size_t varIdx = 0; varIdx < variables.size(); ++varIdx) {
        std::vector<std::vector<double>> magDataForVar;
        std::vector<std::vector<double>> phaseDataForVar;

        for (const auto& row : results) {
            if (row.size() > varIdx + 1) { // +1 because first column is frequency
                double freq = row[0].real();
                std::complex<double> value = row[varIdx + 1];

                // محاسبه دامنه به دسی‌بل
                double magnitude = abs(value) > 1e-20 ? 20*log10(abs(value)) : -200;
                //std::cout << "Value :  " << value << "  Magnityde :  " << magnitude << std::endl;
                magDataForVar.push_back({freq, magnitude});

                // محاسبه فاز به درجه
                double phase = std::arg(value) * 180.0 / M_PI;
                phaseDataForVar.push_back({freq, phase});
            }
        }

        magnitudeData.push_back(magDataForVar);
        phaseData.push_back(phaseDataForVar);
        legendNames.push_back(QString::fromStdString(variables[varIdx]));
    }

    // رسم دامنه با محور X لگاریتمی
    plotter.plotBasicGraphWithMathOps(
        magnitudeData,
        title + " - Magnitude (dB)",
        xAxisTitle,
        "Magnitude (dB)",
        legendNames,
        true // ← فرکانس لاگ
    );

    // رسم فاز با محور X خطی
    plotter.plotBasicGraphWithMathOps(
        phaseData,
        title + " - Phase (degrees)",
        xAxisTitle,
        "Phase (degrees)",
        legendNames,
        false
    );
}