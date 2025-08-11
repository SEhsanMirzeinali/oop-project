#include "AC analysis.h"
#include <Eigen/Dense>
#include <cmath>
#include <iostream>

using namespace Eigen;
using namespace std;

vector<vector<complex<double>>> AC::solve(
    CircuitModel& circuit,
    double startFreq,
    double endFreq,
    int points,
    vector<string> variables
) {
    vector<vector<complex<double>>> results;
    vector<vector<complex<double>>> leftSide;
    vector<complex<double>> rightSide;
    vector<vector<double>> freqData;

    double logStart = log10(startFreq);
    double logEnd = log10(endFreq);
    double step = (logEnd - logStart) / (points - 1);

    for (int i = 0; i < points; ++i) {
        double freq = pow(10, logStart + i * step);
        double omega = 2 * M_PI * freq;

        auto G = createMatG(circuit, omega);
        auto B = createMatB(circuit);
        auto C = createMatC(circuit);
        auto D = createMatD();
        auto E = createMatE(circuit, omega);
        auto J = createMatJ(circuit, omega);

        leftSide = combineLeftSide(G, B, C, D);
        rightSide = combineRightSide(J, E);

        MatrixXcd leftSideMat(leftSide.size(), leftSide[0].size());
        for (size_t i = 0; i < leftSide.size(); ++i) {
            for (size_t j = 0; j < leftSide[i].size(); ++j) {
                leftSideMat(i, j) = leftSide[i][j];
            }
        }

        VectorXcd rightSideVec(rightSide.size());
        for (size_t i = 0; i < rightSide.size(); ++i) {
            rightSideVec(i) = rightSide[i];
        }

        VectorXcd solution = leftSideMat.colPivHouseholderQr().solve(rightSideVec);

        vector<complex<double>> row;
        row.push_back(freq);
        for (int i = 0; i < solution.size(); ++i) {
            row.push_back(solution(i));
        }
        results.push_back(row);
    }

    // Process results for plotting or output
    // ...

    return results;
}

vector<vector<complex<double>>> AC::createMatG(CircuitModel& circuit, double omega) {
    vector<vector<complex<double>>> G;
    int n = circuit.getCountOfNodes();
    G.resize(n, vector<complex<double>>(n, 0.0));
    circuit.setNodesNumber();

    for (const auto& comp : circuit.getComponents()) {
        if (auto resistor = dynamic_pointer_cast<Resistor>(comp)) {
            auto node1 = resistor->getNode1();
            auto node2 = resistor->getNode2();

            int node1Index = node1->getNumber();
            int node2Index = node2->getNumber();

            if (node1Index != -1) {
                G[node1Index][node1Index] += 1.0 / resistor->getResistance();
            }
            if (node2Index != -1) {
                G[node2Index][node2Index] += 1.0 / resistor->getResistance();
            }
            if (node1Index != -1 && node2Index != -1) {
                G[node1Index][node2Index] -= 1.0 / resistor->getResistance();
                G[node2Index][node1Index] -= 1.0 / resistor->getResistance();
            }
        }
        else if (auto capacitor = dynamic_pointer_cast<Capacitor>(comp)) {
            auto node1 = capacitor->getNode1();
            auto node2 = capacitor->getNode2();

            int node1Index = node1->getNumber();
            int node2Index = node2->getNumber();

            complex<double> admittance(0.0, omega * capacitor->getCapacity());

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

            complex<double> admittance(0.0, -1.0 / (omega * inductor->getInductance()));

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

vector<vector<complex<double>>> AC::createMatB(CircuitModel& circuit) {
    vector<vector<complex<double>>> B;
    int n = circuit.getCountOfNodes();
    int m = circuit.getCountOfVoltageSources();
    B.resize(n, vector<complex<double>>(m, 0.0));

    int vs_count = 0;
    for (const auto& comp : circuit.getComponents()) {
        if (auto vs = dynamic_pointer_cast<VoltageSource>(comp)) {
            int node1Index = vs->getNode1()->getNumber();
            int node2Index = vs->getNode2()->getNumber();

            if (node1Index != -1) {
                B[node1Index][vs_count] = 1.0;
            }
            if (node2Index != -1) {
                B[node2Index][vs_count] = -1.0;
            }

            vs_count++;
        }
    }

    return B;
}

vector<vector<complex<double>>> AC::createMatC(CircuitModel& circuit) {
    auto B = createMatB(circuit);
    int n = circuit.getCountOfNodes();
    int m = circuit.getCountOfVoltageSources();
    vector<vector<complex<double>>> C(m, vector<complex<double>>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            C[j][i] = B[i][j];
        }
    }

    return C;
}

vector<vector<complex<double>>> AC::createMatD() {
    // D is typically zero matrix for AC analysis
    int m = 0; // Adjust based on your circuit
    return vector<vector<complex<double>>>(m, vector<complex<double>>(m, 0.0));
}

vector<complex<double>> AC::createMatE(CircuitModel& circuit, double omega) {
    vector<complex<double>> E;
    for (const auto& comp : circuit.getComponents()) {
        if (auto vs = dynamic_pointer_cast<VoltageSource>(comp)) {
            E.push_back(vs->getVoltage());
        }
    }
    return E;
}

vector<complex<double>> AC::createMatJ(CircuitModel& circuit, double omega) {
    int n = circuit.getCountOfNodes();
    vector<complex<double>> J(n, 0.0);
    circuit.setNodesNumber();

    for (const auto& comp : circuit.getComponents()) {
        if (auto cs = dynamic_pointer_cast<CurrentSource>(comp)) {
            int node1Index = cs->getNode1()->getNumber();
            int node2Index = cs->getNode2()->getNumber();

            if (node1Index != -1) {
                J[node1Index] += cs->getCurrent();
            }
            if (node2Index != -1) {
                J[node2Index] -= cs->getCurrent();
            }
        }
    }

    return J;
}

vector<vector<complex<double>>> AC::combineLeftSide(
    const vector<vector<complex<double>>>& G,
    const vector<vector<complex<double>>>& B,
    const vector<vector<complex<double>>>& C,
    const vector<vector<complex<double>>>& D
) {
    size_t n = G.size();
    size_t m = C.size();

    vector<vector<complex<double>>> leftSide(n + m, vector<complex<double>>(n + m, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            leftSide[i][j] = G[i][j];
        }
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            leftSide[i][n + j] = B[i][j];
        }
    }

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            leftSide[n + i][j] = C[i][j];
        }
    }

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            leftSide[n + i][n + j] = D[i][j];
        }
    }

    return leftSide;
}

vector<complex<double>> AC::combineRightSide(
    const vector<complex<double>>& J,
    const vector<complex<double>>& E
) {
    vector<complex<double>> rightSide;
    rightSide.reserve(J.size() + E.size());
    rightSide.insert(rightSide.end(), J.begin(), J.end());
    rightSide.insert(rightSide.end(), E.begin(), E.end());
    return rightSide;
}

void AC::printComplexVector(const vector<vector<complex<double>>>& vec) {
    for (const auto& row : vec) {
        for (const auto& element : row) {
            cout << element << " ";
        }
        cout << endl;
    }
}