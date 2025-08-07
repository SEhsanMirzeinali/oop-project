//
// Created by Purple-Rose on 8/4/2025.
//

#include "ACSweep.h"

#include <vector>
#include <complex>
#include <memory>
#include <Eigen/Dense>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <algorithm>
using namespace Eigen;

std::vector<std::vector<std::complex<double>>> ACSweep::solve(CircuitModel& circuit,double StartFreq ,
    double EndFreq , int numOfPoints ,std::string typeOfSweep,std::vector<std::string> variables) {
    std::vector<std::vector<std::complex<double>>> resultss;
    double currentFreq;
    for (int i = 0; i < numOfPoints; i++) {
        if (typeOfSweep == "Linear") {
            currentFreq = StartFreq + i*(EndFreq - StartFreq)/(numOfPoints-1);
        }
        else if (typeOfSweep == "Octave") {
            currentFreq = StartFreq*pow(2.0,i*log2(EndFreq / StartFreq)/(numOfPoints-1));
        }
        else if (typeOfSweep == "Decade") {
            currentFreq = StartFreq*pow(10.0,i*log10(EndFreq / StartFreq)/(numOfPoints-1));
        }
        //currentTime = TStart + i*TStep;

        for (auto& comp : circuit.getComponents()) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                if (vs->getNode1() && vs->getNode2()) {
                    vs->setOmega(2*3.14*currentFreq);
                    vs->setVoltage();
                }
            }
        }

        std::vector<std::complex<double>> results;
        std::vector<std::vector<std::complex<double>>> leftSide;
        std::vector<std::complex<double>> rightSide;
                //std::cout<<"freq: "<<currentFreq<<std::endl;

        std::vector<std::vector<std::complex<double>>> G = createMatG(circuit);

        std::vector<std::vector<std::complex<double>>> B = createMatB(circuit);
        std::vector<std::vector<std::complex<double>>> C = createMatC(circuit);
        std::vector<std::vector<std::complex<double>>> D = createMatD(circuit);
        std::vector<std::complex<double>> E = createMatE(circuit);
        std::vector<std::complex<double>> J = createMatJ(circuit);

        leftSide = combineLeftSide(G, B, C, D);
        rightSide = combineRightSide(J, E);
         results = solveComplexSystem(leftSide,rightSide);

        // for(int i=0 ; i<rightSide.size() ; i++) {
        //     std::cout<<"rightSide["<<i<<"]: "<<rightSide[i]<<std::endl;
        // }
        // std::cout<<std::endl<<"left side: \n";
        // for(int i=0 ; i<leftSide.size(); i++) {
        //     for(int j=0 ; j<leftSide.size() ; j++) {
        //         std::cout<<leftSide[i][j]<<" ";
        //     }
        //     std::cout<<std::endl;
        // }
        // for(int i=0 ; i<results.size() ; i++) {
        //     std::cout<<"results["<<i<<"]: "<<results[i]<<std::endl;
        // }
        double omega,ampl,phase;
        for (auto& comp : circuit.getComponents()) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                if (vs->getNode1() && vs->getNode2()) {
                    omega = vs->getOmega();
                    phase=vs->getPhase();
                }
            }
        }
        circuitResults->AC_Analysis("AC",results,omega,phase,variables,circuit);
        //printvector(results);
        //circuitResults->Transient_Analyse(results, variables, /*TStart*/+i*TStep,TStep, circuit);
        //std::cout<<"inja\n";

    }
    return resultss;
}
std::vector<std::vector<std::complex<double>>> ACSweep::Phasesolve(CircuitModel& circuit,double baseFreq,
    double StartPhase ,double EndPhase , int numOfPoints ,std::vector<std::string> variables) {
    std::vector<std::vector<std::complex<double>>> resultss;
    for (auto& comp : circuit.getComponents()) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
            if (vs->getNode1() && vs->getNode2()) {
                vs->setOmega(2*3.14*baseFreq);
            }
        }
    }
    double currentPhase;

    for (int i = 0; i < numOfPoints; i++) {
        currentPhase = StartPhase + i*(EndPhase - StartPhase)/(numOfPoints-1);


        for (auto& comp : circuit.getComponents()) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                if (vs->getNode1() && vs->getNode2()) {
                    vs->setPhase(currentPhase);
                    vs->setVoltage();
                }
            }
        }

        std::vector<std::complex<double>> results;
        std::vector<std::vector<std::complex<double>>> leftSide;
        std::vector<std::complex<double>> rightSide;

        std::vector<std::vector<std::complex<double>>> G = createMatG(circuit);

        std::vector<std::vector<std::complex<double>>> B = createMatB(circuit);
        std::vector<std::vector<std::complex<double>>> C = createMatC(circuit);
        std::vector<std::vector<std::complex<double>>> D = createMatD(circuit);
        std::vector<std::complex<double>> E = createMatE(circuit);
        std::vector<std::complex<double>> J = createMatJ(circuit);

        leftSide = combineLeftSide(G, B, C, D);
        rightSide = combineRightSide(J, E);
         results = solveComplexSystem(leftSide,rightSide);

        // for(int i=0 ; i<rightSide.size() ; i++) {
        //     std::cout<<"rightSide["<<i<<"]: "<<rightSide[i]<<std::endl;
        // }
        // std::cout<<std::endl<<"left side: \n";
        // for(int i=0 ; i<leftSide.size(); i++) {
        //     for(int j=0 ; j<leftSide.size() ; j++) {
        //         std::cout<<leftSide[i][j]<<" ";
        //     }
        //     std::cout<<std::endl;
        // }
        // for(int i=0 ; i<results.size() ; i++) {
        //     std::cout<<"results["<<i<<"]: "<<results[i]<<std::endl;
        // }
        double omega,ampl,phase;
        for (auto& comp : circuit.getComponents()) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                if (vs->getNode1() && vs->getNode2()) {
                    omega = vs->getOmega();
                    phase=vs->getPhase();
                }
            }
        }
        circuitResults->AC_Analysis("Phase",results,omega,phase,variables,circuit);
        //printvector(results);
        //circuitResults->Transient_Analyse(results, variables, /*TStart*/+i*TStep,TStep, circuit);
        //std::cout<<"inja\n";

    }
    return resultss;
}


std::vector<std::vector<std::complex<double>>> ACSweep::createMatG(CircuitModel& circuit) {
    int n = circuit.getCountOfNodes();
    // std::vector<std::vector<std::complex<double>>> G;
    // for(int i = 0; i < n; i++) {
    //     G.emplace_back(n, std::complex<double>(0.0,0));
    // }
    std::vector<std::vector<std::complex<double>>> G;
    G.resize(n, std::vector<std::complex<double>>(n, std::complex<double>(0.0,0)));
    double omega = 0.0; // مقدار اولیه برای فرکانس

    // ابتدا فرکانس را از منابع ولتاژ AC پیدا می‌کنیم
    for (const auto& comp : circuit.getComponents()) {
        if (auto v = dynamic_pointer_cast<VoltageSource>(comp)) {
            omega = v->getOmega();
            break; // فرض می‌کنیم همه منابع فرکانس یکسان دارند
        }
    }
    circuit.setNodesNumber();

    for (const auto& comp : circuit.getComponents()) {
        if (auto resistor = dynamic_pointer_cast<Resistor>(comp)) {
            auto node1 = resistor->getNode1();
            auto node2 = resistor->getNode2();
            int node1Index = node1->getNumber();
            int node2Index = node2->getNumber();
            std::complex<double> conductance(resistor->getConductance(), 0.0);

            if (node1Index != -1) {
                G[node1Index][node1Index] += conductance;
            }
            if (node2Index != -1) {
                G[node2Index][node2Index] += conductance;
            }
            if (node1Index != -1 && node2Index != -1) {
                G[node1Index][node2Index] -= conductance;
                G[node2Index][node1Index] -= conductance;
            }
        }
        else if (auto capacitor = dynamic_pointer_cast<Capacitor>(comp)) {
            auto node1 = capacitor->getNode1();
            auto node2 = capacitor->getNode2();
            int node1Index = node1->getNumber();
            int node2Index = node2->getNumber();
            std::complex<double> admittance = 1.0 / capacitor->getImpedance(omega);

            if (node1Index != -1) {
                G[node1Index][node1Index] += admittance;
            }
            if (node2Index != -1) {
                G[node2Index][node2Index] += admittance;
            }
            if (node1Index != -1 && node2Index != -1) {
                G[node1Index][node2Index] -= admittance;
                G[node2Index][node1Index] -= admittance;
            }
        }
        else if (auto inductor = dynamic_pointer_cast<Inductor>(comp)) {
            auto node1 = inductor->getNode1();
            auto node2 = inductor->getNode2();
            int node1Index = node1->getNumber();
            int node2Index = node2->getNumber();
            std::complex<double> admittance = 1.0 / inductor->getImpedance(omega);

            if (node1Index != -1) {
                G[node1Index][node1Index] += admittance;
            }
            if (node2Index != -1) {
                G[node2Index][node2Index] += admittance;
            }
            if (node1Index != -1 && node2Index != -1) {
                G[node1Index][node2Index] -= admittance;
                G[node2Index][node1Index] -= admittance;
            }
        }
    }
    return G;
}


std::vector<std::vector<std::complex<double>>> ACSweep::createMatB(CircuitModel& circuit) {
    int n = circuit.getCountOfNodes();
    int m = circuit.getCountOfVoltageSources();

    // ایجاد ماتریس با نوع مختلط
    std::vector<std::vector<std::complex<double>>> B(
        n,
        std::vector<std::complex<double>>(m, {0.0, 0.0})
    );

    int vs_count = 0;
    for (const auto& component : circuit.getComponents()) {
        if (auto vs = std::dynamic_pointer_cast<VoltageSource>(component)) {
            int node1Index = vs->getNode1()->getNumber();
            int node2Index = vs->getNode2()->getNumber();

            if (node1Index != -1) {
                B[node1Index][vs_count] = {1.0, 0.0};
            }
            if (node2Index != -1) {
                B[node2Index][vs_count] = {-1.0, 0.0};
            }
            vs_count++;
        }
    }

    return B;
}

std::vector<std::vector<std::complex<double>>> ACSweep::createMatC(CircuitModel& circuit) {
    auto B = createMatB(circuit);
    if (B.empty()) return {};

    int n = B.size();
    int m = B[0].size();

    std::vector<std::vector<std::complex<double>>> C(
        m,
        std::vector<std::complex<double>>(n, {0.0, 0.0})
    );

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            C[j][i] = B[i][j];
        }
    }
    return C;
}

std::vector<std::vector<std::complex<double>>> ACSweep::createMatD(CircuitModel& circuit) {
    int m = circuit.getCountOfVoltageSources();


    return std::vector<std::vector<std::complex<double>>>(
        m,
        std::vector<std::complex<double>>(m, {0.0, 0.0})
    );
}




std::vector<std::complex<double>> ACSweep::createMatJ(CircuitModel& circuit) {
    int n = circuit.getCountOfNodes();
    std::vector<std::complex<double>> J(n, {0.0, 0.0}); // مقداردهی اولیه با صفر مختلط
    circuit.setNodesNumber();

    for (auto& comp : circuit.getComponents()) {
        if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
            int node1Index = cs->getNode1()->getNumber();
            int node2Index = cs->getNode2()->getNumber();

            if (node1Index != -1) {
                J[node1Index] += std::complex<double>(cs->getCurrent(), 0.0);
            }
            if (node2Index != -1) {
                J[node2Index] -= std::complex<double>(cs->getCurrent(), 0.0);
            }
        }
    }
    return J;
}
std::vector<std::complex<double>> ACSweep::createMatE(CircuitModel& circuit) {
    int m = circuit.getCountOfVoltageSources();
    std::vector<std::complex<double>> E;
    E.reserve(circuit.getCountOfVoltageSources());
    for (auto& comp : circuit.getComponents()) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
            if (vs->getNode1() && vs->getNode2()) {
                E.push_back(vs->getComplexVoltage());
            }
        }
    }

    // for (const auto& component : circuit.getComponents()) {
    //     if (auto inductor = std::dynamic_pointer_cast<Inductor>(component)) {
    //         E.push_back(0);
    //     }
    // }
    return E;
}

std::vector<std::vector<std::complex<double>>> ACSweep::combineLeftSide(
    const std::vector<std::vector<std::complex<double>>>& G,
    const std::vector<std::vector<std::complex<double>>>& B,
    const std::vector<std::vector<std::complex<double>>>& C,
    const std::vector<std::vector<std::complex<double>>>& D)
{
    size_t n = G.size();
    size_t m = C.size();

    // ایجاد ماتریس ترکیبی با اعداد مختلط
    std::vector<std::vector<std::complex<double>>> leftSide(
        n + m,
        std::vector<std::complex<double>>(n + m, {0.0, 0.0})
    );

    // پر کردن بخش G
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            leftSide[i][j] = G[i][j];
        }
    }

    // پر کردن بخش B
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            leftSide[i][n + j] = B[i][j];
        }
    }

    // پر کردن بخش C
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            leftSide[n + i][j] = C[i][j];
        }
    }

    // پر کردن بخش D
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            leftSide[n + i][n + j] = D[i][j];
        }
    }

    return leftSide;
}

std::vector<std::complex<double>> ACSweep::combineRightSide(
    const std::vector<std::complex<double>>& J,  // n × 1
    const std::vector<std::complex<double>>& E)  // m × 1
{
    std::vector<std::complex<double>> rightSide;
    rightSide.reserve(J.size() + E.size());

    // ترکیب بردارهای J و E
    rightSide.insert(rightSide.end(), J.begin(), J.end());
    rightSide.insert(rightSide.end(), E.begin(), E.end());

    return rightSide;
}
std::vector<std::complex<double>> ACSweep::solveComplexSystem(const std::vector<std::vector<std::complex<double>>>& A,
                                         const std::vector<std::complex<double>>& b) {
    // تبدیل به ماتریس Eigen
    size_t n = A.size();
    MatrixXcd A_mat(n, n);
    VectorXcd b_vec(n);

    for(size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < n; ++j) {
            A_mat(i,j) = A[i][j];
        }
        b_vec(i) = b[i];
    }

    // حل سیستم معادلات
    VectorXcd x = A_mat.partialPivLu().solve(b_vec);

    // تبدیل نتیجه به vector
    std::vector<std::complex<double>> result(n);
    for(size_t i = 0; i < n; ++i) {
        result[i] = x(i);
    }

    return result;
}