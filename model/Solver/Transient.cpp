//
// Created by Purple-Rose on 6/2/2025.
//

#include "Transient.h"
#include <Eigen/Dense>

using namespace Eigen;

std::vector<std::vector<double>> Transient::solve(CircuitModel& circuit,double TStep,double TStop,double TStart, double TMax,std::vector<std::string> variables){
  std::vector<std::vector<double>> results;
  std::vector<std::vector<double>> leftSide;
  std::vector<double> rightSide;
  std::vector<std::vector<double>> G=createMatG(circuit);
  std::vector<std::vector<double>> B=createMatB(circuit);
  std::vector<std::vector<double>> C=createMatC(circuit);
  std::vector<std::vector<double>> D=createMatD(circuit);
  std::vector<double> E=createMatE(circuit);
  std::vector<double> J=createMatJ(circuit);
  std::vector<std::vector<double>> midSide;


  leftSide=combineLeftSide(G,B,C,D);
  rightSide=combineRightSide(J,E);
  midSide=createMatDynamic(circuit);
  std::vector<double> X0(leftSide.size(),0);;
  results=solveMatrixODE(leftSide,midSide,rightSide,X0,TStep,TStop);
  //PRINT

  // std::cout<<"leftSide:\n";
  // printvector(leftSide);
  // std::cout<<"rightSide:\n";
  // for(auto it = rightSide.begin(); it != rightSide.end(); ++it) {
  //   std::cout<<*it<<std::endl;
  // }
  // std::cout<<"results:\n";
  // printvector(results);

  circuitResults->Transient_Analyse(results,variables,TStart,circuit);
  std::cout<<"done3\n";

 return results;

}
void Transient::printvector(const std::vector<std::vector<double>>& vec) {
  for (const auto& row : vec) {
    for (const auto& element : row) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }
}
    std::vector<std::vector<double>> Transient::createMatG( CircuitModel& circuit){
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
   std::vector<std::vector<double>> Transient:: createMatB(CircuitModel& circuit) {
  std::vector<std::vector<double>> B;

  int n=circuit.getCountOfNodes();
  int m=circuit.getCountOfVoltageSources();
  for (const auto& component : circuit.getComponents()) {
    if (auto inductor = std::dynamic_pointer_cast<Inductor>(component)) {
      m++;
    }
  }
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
  int L_count=vs_count;
  for (size_t idx = 0; idx < circuit.getComponents().size(); ++idx) {
    if (auto vs = dynamic_cast<Inductor*>(circuit.getComponents()[idx].get())) {
      int node1Index = vs->getNode1()->getNumber();
      int node2Index = vs->getNode2()->getNumber();

      if (node1Index != -1) {
        B[node1Index][L_count] = 1;
      }
      if (node2Index != -1) {
        B[node2Index][L_count] = -1;
      }

      L_count++;
    }
  }

return B;
}

std::vector<std::vector<double>> Transient:: createMatC(CircuitModel& circuit) {
  std::vector<std::vector<double>> C;
  std::vector<std::vector<double>> B=createMatB(circuit);
    int n=circuit.getCountOfNodes();
  int m=circuit.getCountOfVoltageSources();
  for (const auto& component : circuit.getComponents()) {
    if (auto inductor = std::dynamic_pointer_cast<Inductor>(component)) {
      m++;
    }
  }
 C.resize(m, std::vector<double>(n, 0));

    // انجام عمل ترانهاده
    for(size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < m; ++j) {
            C[j][i] = B[i][j];
        }
    }

return C;
}
std::vector<std::vector<double>> Transient:: createMatD(CircuitModel& circuit) {
  int m=circuit.getCountOfVoltageSources();
  for (const auto& component : circuit.getComponents()) {
    if (auto inductor = std::dynamic_pointer_cast<Inductor>(component)) {
      m++;
    }
  }
  std::vector<std::vector<double>> D;
  D.resize(m, std::vector<double>(m, 0));
  return D;
}
    std::vector<double> Transient:: createMatE( CircuitModel& circuit){
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
  //=====================================
  for (const auto& component : circuit.getComponents()) {
    if (auto inductor = std::dynamic_pointer_cast<Inductor>(component)) {
      E.push_back(0);
    }
  }
      return E;
    }
std::vector<double> Transient:: createMatJ( CircuitModel& circuit){
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
std::vector<std::vector<double>> Transient::combineLeftSide(
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
std::vector<double> Transient::combineRightSide(
    const std::vector<double>& J,  // n × 1
    const std::vector<double>& E)  // m × 1
{
  std::vector<double> rightSide;
  rightSide.reserve(J.size() + E.size());

  rightSide.insert(rightSide.end(), J.begin(), J.end());
  rightSide.insert(rightSide.end(), E.begin(), E.end());

  return rightSide;
}
std::vector<std::vector<double> > Transient::createMatDynamic(CircuitModel &circuit) {
  std::vector<std::vector<double>> K;
  int n=circuit.getCountOfNodes();
  int m=circuit.getCountOfVoltageSources();
  int l=0;
  for (const auto& comp : circuit.getComponents()) {
    if (auto inductor = std::dynamic_pointer_cast<Inductor>(comp)) {
      m++;
      l++;
    }
  }
  K.resize(n+m,std::vector<double>(n+m,0));
  circuit.setNodesNumber();
  for (const auto& comp : circuit.getComponents()) {
    if (auto capacitor = std::dynamic_pointer_cast<Capacitor>(comp)) {
      auto node1 = capacitor->getNode1();
      auto node2 = capacitor->getNode2();

      int node1Index = node1->getNumber();
      int node2Index = node2->getNumber();

      if (node1Index != -1) {
        K[node1Index][node1Index] += capacitor->getCapacity();
      }
      if (node2Index != -1) {
        K[node2Index][node2Index] += capacitor->getCapacity();
      }
      if (node1Index != -1 && node2Index != -1) {
        K[node1Index][node2Index] -= capacitor->getCapacity();
        K[node2Index][node1Index] -= capacitor->getCapacity();
      }
    }
  }
  for(int i=m+n-l; i<m+n; ++i) {
    for(auto& comp : circuit.getComponents()) {
      if (auto inductor = std::dynamic_pointer_cast<Inductor>(comp)) {
        K[i][i]=(-1*inductor->getInductance());
      }
    }
  }

// std::cout<<"dynamic :\n";
//   printvector(K);
  return K;
}
std::vector<std::vector<double>> Transient:: solveMatrixODE(const std::vector<std::vector<double>>& G,
                                    const std::vector<std::vector<double>>& C,
                                    const std::vector<double>& F,
                                    const std::vector<double>& X0,
                                    double dt,
                                    double t_end) {

  MatrixXd G_mat(G.size(), G[0].size());
  MatrixXd C_mat(C.size(), C[0].size());

  for (size_t i = 0; i < G.size(); ++i)
    for (size_t j = 0; j < G[i].size(); ++j)
      G_mat(i,j) = G[i][j];

  for (size_t i = 0; i < C.size(); ++i)
    for (size_t j = 0; j < C[i].size(); ++j)
      C_mat(i,j) = C[i][j];

  Map<const VectorXd> F_vec(F.data(), F.size());
  Map<const VectorXd> X0_vec(X0.data(), X0.size());

  int n_steps = static_cast<int>(t_end / dt);
  std::vector<std::vector<double>> solution_history;
  solution_history.reserve(n_steps + 1);

  VectorXd X_curr = X0_vec;
  VectorXd X_prev = X0_vec;

  // ذخیره شرایط اولیه
  solution_history.push_back(X0);

  // حل در هر گام زمانی
  for (int i = 1; i <= n_steps; ++i) {
    MatrixXd A = G_mat + C_mat / dt;
    VectorXd b = F_vec + (C_mat / dt) * X_prev;
    X_curr = A.colPivHouseholderQr().solve(b);

    // ذخیره جواب فعلی
    std::vector<double> current_solution(X_curr.data(), X_curr.data() + X_curr.size());
    solution_history.push_back(current_solution);

    X_prev = X_curr;
  }

  // اضافه کردن ستون زمان
  for (size_t i = 0; i < solution_history.size(); ++i) {
    double time = i * dt;
    solution_history[i].insert(solution_history[i].begin(), time);
  }

  return solution_history;
}
std::vector<std::vector<double>> addTimeColumn(const std::vector<std::vector<double>>& solution, double dt) {
  std::vector<std::vector<double>> solution_with_time;
  solution_with_time.reserve(solution.size());

  for (size_t i = 0; i < solution.size(); ++i) {
    std::vector<double> row;
    row.reserve(solution[i].size() + 1);
    row.push_back(i * dt); // زمان
    row.insert(row.end(), solution[i].begin(), solution[i].end()); // مقادیر متغیرها
    solution_with_time.push_back(row);
  }

  return solution_with_time;
}
