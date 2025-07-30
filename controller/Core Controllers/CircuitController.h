//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef CIRCUITCONTROLLER_H
#define CIRCUITCONTROLLER_H

#include <string>

#include "../../model/Circuit Model/CircuitModel.h"
#include"../../model/Solver/DCSolverTest.h"
#include"../../model/Solver/Transient.h"
#include "../../model/Circuit Model/components/Capacitor.h"
#include "../../model/Circuit Model/components/Resistor.h"
#include "../../model/Circuit Model/components/Inductor.h"
#include "../../model/Circuit Model/components/VoltageSource.h"
#include "../../model/Circuit Model/components/CurrentSource.h"

class CircuitController {
private:
    std::shared_ptr<CircuitModel> circuit;
    std::shared_ptr<DCSolverTest> DCAnalyse;
    std::shared_ptr<Transient> transientAnalyse;
public:
    //CircuitController();

    CircuitController() : circuit(std::make_shared<CircuitModel>()) , DCAnalyse(std::make_shared<DCSolverTest>())
    ,transientAnalyse(std::make_shared<Transient>()){}
    ////////////////////////////////// Fotuhi
    void get_Resistors(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value);
    void get_Capacitors(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value);
    void get_Inductor(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value);
    void get_diode(std::string name , std::string node1 , std::string node2 , std::string model);
    void get_Ground(std::string node);
    void get_Deleted_element(std::string name , char type);
    void get_Ungrounded_node(std::string node);
    void get_Voltage_source(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value);
    void get_Current_source(std::string name , std::string node1 , std::string node2 , std::string string_value , double double_value);


    void get_SIN_voltage(std::string name , std::string node1 , std::string node2 , std::string Vofset_string ,
        double Vofset_double , std::string Vamp_string , double Vamp_double , std::string Freq_string , double Freq_double) ;
    void get_VCVS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 ,std::string CtrN2
        , std::string Gain_string , double Gain_double) ;
    void get_VCCS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 ,std::string CtrN2
        , std::string Gain_string , double Gain_double) ;
    void get_CCVS(std::string name , std::string node1 , std::string node2 , std::string Vname, std::string Gain_string
        , double Gain_double) ;
    void get_CCCS(std::string name , std::string node1 , std::string node2 , std::string Vname, std::string Gain_string
         , double Gain_double) ;
    void get_TRAN(std::string Tstep_string , double Tstep_double , std::string Tstop_string ,double Tstop_double
        , std::string Tstart_string , double Tstart_double , std::string Tmaxstep_string ,double Tmaxstep_double);

////////////////////////////mir
    void DC_solve();
    void tran_solve(double dt,double TStop,double TStart,double TMax_step,std::vector<std::string> namesAndVI);
    void print_All_nodes() const;
    void print_All_elements() const;
    void print_element(std::string type);
    void change_node_name(std::string old,std::string newName);

    ////////////////////////////////// end

    // Add a new component
/*    void addComponent(ComponentType type, double value, int node1, int node2) {
        Component* component;
        switch (type) {
            case RESISTOR: component = new Resistor(value); break;
            case CAPACITOR: component = new Capacitor(value); break;
            // ... other component types
        }
        circuit.addComponent(component, node1, node2);
    }

    // Remove a component by ID
    void removeComponent(int componentId) {
        circuit.removeComponent(componentId);
    }

    // Update component value
    void updateComponent(int componentId, double newValue) {
        circuit.getComponent(componentId).setValue(newValue);
    }
*/
};



#endif //CIRCUITCONTROLLER_H
