//
// Created by Purple-Rose on 6/3/2025.
//

#include "DCSolverTest.h"
#include<iostream>

void DCSolverTest::initializeTestCircuit(){


}
std::vector<double> DCSolverTest::solve(CircuitModel& circuit){
  std::vector<double> results;
  std::vector<std::vector<double>> leftSide;
  std::vector<double> rightSide;
  std::vector<std::vector<double>> G=createMatG(circuit);
  std::vector<std::vector<double>> B=createMatB(circuit);
  std::vector<std::vector<double>> C=createMatC(circuit);
  std::vector<std::vector<double>> D=createMatD(circuit);
  std::vector<double> E=createMatE(circuit);
  std::vector<double> J=createMatJ(circuit);
//  circuit.setNodesNumber();
  leftSide=combineLeftSide(G,B,C,D);
  rightSide=combineRightSide(J,E);
  results=solveLinearSystem(leftSide,rightSide);
  //PRINT
  // std::cout<<"leftSide:\n";
  // printvector(leftSide);
  // std::cout<<"rightSide:\n";
  // for(auto it = rightSide.begin(); it != rightSide.end(); ++it) {
  //   std::cout<<*it<<std::endl;
  // }
  // std::cout<<"results:\n";
  // for(auto it = results.begin(); it != results.end(); ++it) {
  //
  //   std::cout<<*it<<std::endl;
  // }
  circuitResults->DC_Analyse_Results(results,circuit);
  return results;

}
void DCSolverTest::printvector(const std::vector<std::vector<double>>& vec) {
  for (const auto& row : vec) {
    for (const auto& element : row) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }
}
    std::vector<std::vector<double>> DCSolverTest::createMatG( CircuitModel& circuit){
  std::vector<std::vector<double>> G;
  int n=circuit.getCountOfNodes();
  G.resize(n,std::vector<double>(n,0));
  circuit.setNodesNumber();
  for (const auto& comp : circuit.getComponents()) {
    if (auto resistor = std::dynamic_pointer_cast<Resistor>(comp)) {
      auto node1 = resistor->getNode1();
      auto node2 = resistor->getNode2();

      int node1Index = node1->getNumber();
      int node2Index = node2->getNumber();

      if (node1Index != -1) {
        G[node1Index][node1Index] += resistor->getConductance();
      }
      if (node2Index != -1) {
        G[node2Index][node2Index] += resistor->getConductance();
      }
      if (node1Index != -1 && node2Index != -1) {
        G[node1Index][node2Index] -= resistor->getConductance();
        G[node2Index][node1Index] -= resistor->getConductance();
      }
    }
  }

return G;

}
   std::vector<std::vector<double>> DCSolverTest:: createMatB(CircuitModel& circuit) {
  std::vector<std::vector<double>> B;

  int n=circuit.getCountOfNodes();
  int m=circuit.getCountOfVoltageSources();
  B.resize(n,std::vector<double>(m,0));

  int vs_count = 0;
  for (size_t idx = 0; idx < circuit.getComponents().size(); ++idx) {
    if (auto vs = dynamic_cast<VoltageSource*>(circuit.getComponents()[idx].get())) {
      int node1Index = vs->getNode1()->getNumber();
      int node2Index = vs->getNode2()->getNumber();

      if (node1Index != -1) {
        B[node1Index][vs_count] = 1;
      }
      if (node2Index != -1) {
        B[node2Index][vs_count] = -1;
      }

      vs_count++;
    }
  }
return B;
}

std::vector<std::vector<double>> DCSolverTest:: createMatC(CircuitModel& circuit) {
  std::vector<std::vector<double>> C;
  std::vector<std::vector<double>> B=createMatB(circuit);
    int n=circuit.getCountOfNodes();
  int m=circuit.getCountOfVoltageSources();
 C.resize(m, std::vector<double>(n, 0));

    // انجام عمل ترانهاده
    for(size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < m; ++j) {
            C[j][i] = B[i][j];
        }
    }

return C;
}
std::vector<std::vector<double>> DCSolverTest:: createMatD(CircuitModel& circuit) {
  int m=circuit.getCountOfVoltageSources();
  std::vector<std::vector<double>> D;
  D.resize(m, std::vector<double>(m, 0));
  return D;
}
    std::vector<double> DCSolverTest:: createMatE( CircuitModel& circuit){
      int m=circuit.getCountOfVoltageSources();
      std::vector<double> E;
       E.reserve(circuit.getCountOfVoltageSources());
for (auto& comp : circuit.getComponents()) {
    if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
		if (vs->getNode1() && vs->getNode2()) {
                E.push_back(vs->getVoltage());
            }
     }
  }
      return E;
    }
std::vector<double> DCSolverTest:: createMatJ( CircuitModel& circuit){
  int n=circuit.getCountOfNodes();
  std::vector<double> J;
  J.resize(n, 0);
  circuit.setNodesNumber();
  for (auto& comp : circuit.getComponents()) {
    if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
      int node1Index=cs->getNode1()->getNumber();
      int node2Index=cs->getNode2()->getNumber();
      //جریان خارج شونده
      if(node1Index!=-1) {
        J[node1Index]+=cs->getCurrent();
      }
      //جریان وارد شونده
      if(node2Index!=-1) {
        J[node2Index]-= cs->getCurrent();
      }
     }
  }
  return J;
}
std::vector<std::vector<double>> DCSolverTest::combineLeftSide(
    const std::vector<std::vector<double>>& G,
    const std::vector<std::vector<double>>& B,
    const std::vector<std::vector<double>>& C,
    const std::vector<std::vector<double>>& D)
{
  size_t n = G.size();
  size_t m = C.size();

  std::vector<std::vector<double>> leftSide(n + m, std::vector<double>(n + m, 0.0));

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
std::vector<double> DCSolverTest::combineRightSide(
    const std::vector<double>& J,  // n × 1
    const std::vector<double>& E)  // m × 1
{
  std::vector<double> rightSide;
  rightSide.reserve(J.size() + E.size());

  rightSide.insert(rightSide.end(), J.begin(), J.end());
  rightSide.insert(rightSide.end(), E.begin(), E.end());

  return rightSide;
}
std::vector<double> DCSolverTest::solveLinearSystem(std::vector<std::vector<double>> A, std::vector<double> b) {
  int n = A.size();

  for (int i = 0; i < n; ++i) {
    A[i].push_back(b[i]);
  }

  for (int i = 0; i < n; ++i) {
    double maxEl = abs(A[i][i]);
    int maxRow = i;
    for (int k = i + 1; k < n; ++k) {
      if (abs(A[k][i]) > maxEl) {
        maxEl = abs(A[k][i]);
        maxRow = k;
      }
    }

    swap(A[i], A[maxRow]);

    for (int k = i + 1; k < n; ++k) {
      double c = -A[k][i] / A[i][i];
      for (int j = i; j <= n; ++j) {
        if (i == j)
          A[k][j] = 0;
        else
          A[k][j] += c * A[i][j];
      }
    }
  }

  std::vector<double> x(n);
  for (int i = n - 1; i >= 0; --i) {
    x[i] = A[i][n] / A[i][i];
    for (int k = i - 1; k >= 0; --k) {
      A[k][n] -= A[k][i] * x[i];
    }
  }

  return x;
}



