//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef CIRCUITCONTROLLER_H
#define CIRCUITCONTROLLER_H

#include <string>

#include "ACSweep.h"
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
    std::shared_ptr<ACSweep> acAnalyse;

public:
    //CircuitController();

    CircuitController() : circuit(std::make_shared<CircuitModel>()) , DCAnalyse(std::make_shared<DCSolverTest>())
    ,transientAnalyse(std::make_shared<Transient>()),acAnalyse(std::make_shared<ACSweep>()){}
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


    void get_AC_voltage(std::string name, std::string node1, std::string node2,double ampl, double phase);
    void get_SIN_voltage(std::string name, std::string node1, std::string node2,
        double offset, double ampl, double freq,double TDelay,double theta, double phase ,double cycles);
    void get_PULSE_voltage(std::string name, std::string node1, std::string node2,
    double init, double von, double delay,double rise,double fall, double ton,double period,double cycles);
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
    void ac_solve(double startFreq , double endFreq ,
            int numOfPoints ,std::string typeOfSweep,std::vector<std::string> namesAndVI);
    void phase_solve(double baseFreq ,double startPhase, double endPhase ,
        int numOfPoints ,std::vector<std::string> namesAndVI);

    void print_All_nodes() const;
    void print_All_elements() const;
    void print_element(std::string type);
    void change_node_name(std::string old,std::string newName);

    ////////////////////////////////// end

};



#endif //CIRCUITCONTROLLER_H
