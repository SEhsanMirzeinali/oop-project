//
// Created by Purple-Rose on 6/2/2025.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#pragma once
#include <vector>
#include "../../controller/Core Controllers/CircuitController.h"
#include "../../controller/Supporting Controllers/IOController.h"

class CommandHandler {
    public:
    static std::vector<IOController> Files;

    CircuitController Circuitcontroller;
    double Change_unitsValue(const std::string& input);
    void handle_Add_resistance(std::string name  , std::string node1 , std::string node2 , std::string value_string);
    void handle_Add_capacitor(std::string name  , std::string node1 , std::string node2 , std::string value_string);
    void handle_Add_inductor(std::string name  , std::string node1 , std::string node2 , std::string value_string);
    void handle_Add_VoltageSource(std::string name  , std::string node1 , std::string node2 , std::string value_string);
    void handle_Add_CurrentSource(std::string name  , std::string node1 , std::string node2 , std::string value_string);
    void handle_Add_Diod(std::string name  , std::string node1 , std::string node2 , std::string model);
    void handle_Add_Ground(std::string node);
    void handle_Delete_element(std::string name , char RLC);
    void handle_Delete_ground(std::string node);
    static std::vector<std::string> names;
    void handle_Add_SIN_voltage(std::string name , std::string node1 , std::string node2 , std::string Vofset , std::string Vamp , std::string Freq);

    void handle_Add_VCVS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 , std::string CtrN2 , std::string Gain);
    void handle_Add_VCCS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 , std::string CtrN2 , std::string Gain);
    void handle_Add_CCVS(std::string name , std::string node1 , std::string node2 , std::string Vname , std::string Gain);
    void handle_Add_CCCS(std::string name , std::string node1 , std::string node2 , std::string Vname , std::string Gain);
    void handle_Add_TRAN(std::string Tstep , std::string Tstop , std::string Tstart , std::string Tmaxstep);

    ///////////////////////////////mir
    void handle_DC_Analysis();
    void handle_Tran_Analysis(std::string dt,std::string Tstop,std::string Tstart,std::string Tmax_step,std::string variables);
    void handle_print_all_nodes();
    void handle_print_All_elements();
    void handle_print_element(std::string type);
    void handle_change_node_name(std::string old_name,std::string new_name);
};



#endif //COMMANDHANDLER_H
