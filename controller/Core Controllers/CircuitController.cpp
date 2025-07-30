//
// Created by Purple-Rose on 6/2/2025.
//

#include "CircuitController.h"

#include <iostream>


void CircuitController::get_Resistors(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value){
    std::shared_ptr<Node> n1 = circuit->addNode(node1);
    std::shared_ptr<Node> n2 = circuit->addNode(node2);

    auto resistor = std::make_shared<Resistor>(double_value,name,n1.get(),n2.get());
    for (int i=0 ; i<circuit->getComponents().size() ; i++) {
        if(resistor == circuit->getComponents()[i]) {
            return;
        }
    }
    circuit->addComponent(resistor);
//    std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;

    // if (auto res = dynamic_cast<Resistor*>(circuit->getComponents()[0].get())) {
    //     std::cout<<res->getName()<<std::endl;
    // }
}
void CircuitController::get_Capacitors(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value){
    std::shared_ptr<Node> n1 = circuit->addNode(node1);
    std::shared_ptr<Node> n2 = circuit->addNode(node2);

    auto capacitor = std::make_shared<Capacitor>(double_value,name,n1.get(),n2.get());
    circuit->addComponent(capacitor);
//    std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;

    // if (auto res = dynamic_cast<Capacitor*>(circuit->getComponents()[1].get())) {
    //     std::cout<<res->getNode1()->getName()<<std::endl;
    // }
}
void CircuitController::get_Inductor(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value){
    std::shared_ptr<Node> n1 = circuit->addNode(node1);
    std::shared_ptr<Node> n2 = circuit->addNode(node2);

    auto inductor = std::make_shared<Inductor>(double_value,name,n1.get(),n2.get());
    circuit->addComponent(inductor);
 //   std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;
}
void CircuitController::get_diode(std::string name , std::string node1 , std::string node2 , std::string model) {
}
void CircuitController::get_Ground(std::string node) {

    int numOfGNd=0;
    for (int i=0 ; i<circuit->getNode().size() ; i++) {
        if(circuit->getNode()[i]->isGround()) {
            numOfGNd++;
        }
    }
    if(numOfGNd>1) {
        std::cerr<<"Warning: you have more than one ground node.\n"<<std::endl;
    }
    circuit->setRefrenceNode(node);
//    std::cout<<"size: "<<circuit->getNode().size()<<std::endl;
}
void CircuitController::get_Deleted_element(std::string name , char type) {
    circuit->removeComponent(name);
//    std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;
}
void CircuitController::get_Ungrounded_node(std::string node) {

    int numOfGNd=0;
    for (int i=0 ; i<circuit->getNode().size() ; i++) {
        if(circuit->getNode()[i]->isGround()) {
            numOfGNd++;
        }
    }
    if(numOfGNd==0) {
        std::cerr<<"Error: Ground node not found"<<std::endl;
    }
    circuit->setNotRefrenceNode(node);
//    std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;
}
void CircuitController::get_Voltage_source(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value) {
    std::shared_ptr<Node> n1 = circuit->addNode(node1);
    std::shared_ptr<Node> n2 = circuit->addNode(node2);

    auto v = std::make_shared<VoltageSource>(double_value,name,n1.get(),n2.get());
    circuit->addComponent(v);
  //  std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;
}
void CircuitController::get_Current_source(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value) {
    std::shared_ptr<Node> n1 = circuit->addNode(node1);
    std::shared_ptr<Node> n2 = circuit->addNode(node2);

    auto current = std::make_shared<CurrentSource>(double_value,name,n1.get(),n2.get());
    circuit->addComponent(current);
   // std::cout<<"size: "<<circuit->getComponents().size()<<std::endl;
}

void CircuitController::get_SIN_voltage(std::string name , std::string node1 , std::string node2 , std::string Vofset_string ,
    double Vofset_double , std::string Vamp_string , double Vamp_double , std::string Freq_string , double Freq_double ) {
}
void CircuitController::get_VCVS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 ,std::string CtrN2
    , std::string Gain_string , double Gain_double) {
}
void CircuitController::get_VCCS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 ,std::string CtrN2
    , std::string Gain_string , double Gain_double) {
}
void CircuitController::get_CCVS(std::string name , std::string node1 , std::string node2 , std::string Vname, std::string Gain_string
    , double Gain_double) {
}
void CircuitController::get_CCCS(std::string name , std::string node1 , std::string node2 , std::string Vname, std::string Gain_string
    , double Gain_double) {
}
void CircuitController::get_TRAN(std::string Tstep_string , double Tstep_double , std::string Tstop_string ,double Tstop_double
    , std::string Tstart_string , double Tstart_double , std::string Tmaxstep_string ,double Tmaxstep_double) {
}
//////////////////////////mir
void CircuitController::DC_solve() {
    DCAnalyse->solve(*circuit);
}
void CircuitController::tran_solve(double dt,double TStop,double TStart,double TMax_step,std::vector<std::string> namesAndVI) {
    std::cout<<"count: "<<circuit->getComponents().size()<<std::endl;
    int numOfGNd=0;
    for (int i=0 ; i<circuit->getNode().size() ; i++) {
        if(circuit->getNode()[i]->isGround()) {
            numOfGNd++;
        }
    }
    if(numOfGNd==0) {
        std::cerr<<"Error: Ground node not found"<<std::endl;
        return;
    }
    if(numOfGNd>1) {
        std::cerr<<"Error: you have more than one ground node.\n"<<std::endl;
        return;
    }
    // std::cout<<"dt:"<<dt<<std::endl;
    // std::cout<<"TStop:"<<TStop<<std::endl;
    // std::cout<<"TStart:"<<TStart<<std::endl;
    // std::cout<<"TMax_step:"<<TMax_step<<std::endl;
    bool exist=false;
    for(const auto & j : namesAndVI) {
        for(int i=0 ; i<circuit->getNode().size() ; i++) {
            if(circuit->getNode()[i]->getName()==j.substr(1)) {
                exist=true;
            }
        }
        for(int i=0 ; i<circuit->getComponents().size() ; i++) {

            if(circuit->getComponents()[i]->getName()==j.substr(1)) {
                exist=true;
            }
        }
        if(!exist) {
            std::cerr <<j.substr(1)<<" not found in circuit"<<std::endl;
            return;
        }
        exist=false;
        //std::cout<<"name:"<<j<<std::endl;
    }
    if (circuit == nullptr || transientAnalyse == nullptr) {
        std::cerr << "Error: Null pointer in transient analysis!" << std::endl;
        return;
    }

    std::cout<<std::endl;
    transientAnalyse->solve(*circuit,dt,TStop,TStart,TMax_step,namesAndVI);

    std::cout<<"done2\n";
    return;
}
void CircuitController::print_All_nodes() const {
    std::cout<<"\nAvailable nodes:\n";
    for(int i=0 ; i<circuit->getNode().size() ; i++) {
        std::cout<<circuit->getNode()[i]->getName()<<", ";
    }
    std::cout<<std::endl;
}
void CircuitController::print_All_elements() const {
    std::cout<<"\nAvailable elements:\n";
    for (const auto& comp : circuit->getComponents()) {
        if (auto resistor = std::dynamic_pointer_cast<Resistor>(comp)) {
            std::cout<<resistor->getName()<<": resistance-> "<<resistor->getResistance()<<" ohms"<<std::endl;
        }
        if(auto voltage = std::dynamic_pointer_cast<VoltageSource>(comp)) {
            std::cout<<voltage->getName()<<": voltage-> "<<voltage->getVoltage()<<" volts\n";
        }
        if(auto current = std::dynamic_pointer_cast<CurrentSource>(comp)) {
            std::cout<<current->getName()<<": current-> "<<current->getCurrent()<<" amper\n";
        }
        if(auto c=std::dynamic_pointer_cast<Capacitor>(comp)) {
            std::cout<<c->getName()<<": capacity-> "<<c->getCapacity()<<" Farad\n";
        }
        if(auto l=std::dynamic_pointer_cast<Inductor>(comp)) {
            std::cout<<l->getName()<<": inductance-> "<<l->getInductance()<<std::endl;
        }
    }
}
void CircuitController::print_element(std::string type) {
    std::cout<<"\nAvailable "<<type<<" elements:\n";
    for (const auto& comp : circuit->getComponents()) {
        if(type=="Resistor") {
            if (auto resistor = std::dynamic_pointer_cast<Resistor>(comp)) {
                std::cout<<resistor->getName()<<": resistance-> "<<resistor->getResistance()<<" ohms"<<std::endl;
            }
        }
        else if(type=="VoltageSource") {
            if(auto voltage = std::dynamic_pointer_cast<VoltageSource>(comp)) {
                std::cout<<voltage->getName()<<": voltage-> "<<voltage->getVoltage()<<" volts\n";
            }
        }
        else if(type=="CurrentSource") {
            if(auto current = std::dynamic_pointer_cast<CurrentSource>(comp)) {
                std::cout<<current->getName()<<": current-> "<<current->getCurrent()<<" amper\n";
            }
        }
        else if(type=="Capacitor") {
            if(auto c=std::dynamic_pointer_cast<Capacitor>(comp)) {
                std::cout<<c->getName()<<": capacity-> "<<c->getCapacity()<<" Farad\n";
            }
        }
        else if(type=="Inductor") {
            if(auto l=std::dynamic_pointer_cast<Inductor>(comp)) {
                std::cout<<l->getName()<<": inductance-> "<<l->getInductance()<<std::endl;
            }
        }
    }
}
void CircuitController::change_node_name(std::string old, std::string newName) {
    Node* n=nullptr;
    bool newNameExist=false;

    for(int i=0 ; i<circuit->getNode().size() ; i++) {
        if(circuit->getNode()[i]->getName()==old) {
            n=circuit->getNode()[i].get();
        }
        if(circuit->getNode()[i]->getName()==newName) {
            newNameExist=true;
        }
    }
    if(n==nullptr) {
        std::cerr<< "ERROR: Node "<<old<<" does not exist in the circuit"<<std::endl;
        std::cout<<"\n";
        print_All_nodes();
        return;
    }
    if(newNameExist) {
        std::cerr<<"ERROR: Node name "<<newName<<" already exists"<<std::endl;
        std::cout<<"\n";
        print_All_nodes();
        return;
    }
    n->setName(newName);
    std::cout<<"\nSUCCESS: Node renamed from "<<old<< " to "<<newName<<std::endl;
}




