//
// Created by Purple-Rose on 6/2/2025.
//

#include "CommandHandler.h"
#include<exception>
#include "../Get_Ecxeption.h"
#include<iostream>
#include<regex>
std::vector<IOController> CommandHandler::Files;
std::vector<std::string> CommandHandler::names;

double CommandHandler::Change_unitsValue(const std::string& input) {
    std::regex pattern(R"(^([+-]?\d+(\.\d+)?([eE][+-]?\d+)?)([a-zA-Z]*)$)");
    std::smatch match;

    if (!std::regex_match(input, match, pattern)) {
        throw std::invalid_argument("Invalid input format: " + input);
    }

    double base = std::stod(match[1]);
    std::string suffix = match[4];

    if (suffix == "" || suffix == " ") {
        return base;
    } else if (suffix == "u" || suffix == "U") {
        return base * 1e-6;
    } else if (suffix == "m") {
        return base * 1e-3;
    } else if (suffix == "k" || suffix == "K") {
        return base * 1e3;
    } else if (suffix == "Meg" || suffix == "meg" || suffix == "MEG" || suffix == "M") {
        return base * 1e6;
    } else if (suffix == "G" || suffix == "g") {
        return base * 1e9;
    } else if (suffix == "n") {
        return base * 1e-9;
    } else if (suffix == "p") {
        return base * 1e-12;
    } else {
        throw std::invalid_argument("Unknown unit suffix: " + suffix);
    }
}

void CommandHandler::handle_Add_resistance(std::string name  , std::string node1 , std::string node2 , std::string value_string) {
    name = 'R' + name ;
    for(auto n : names) {
        if(name == n) {
            //throw Get_Exception::Duplicate_Name_Exception(name);
            //return;
        }
    }
    double value_double = Change_unitsValue(value_string);
    if (value_double <= 0) {
        throw Get_Exception::Invalid_Value_Exception();
    }
    names.push_back(name);
    Circuitcontroller.get_Resistors(name , node1 , node2 , value_string , value_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << value_double <<"  Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << value_string << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_capacitor(std::string name  , std::string node1 , std::string node2 , std::string value_string) {
    name = 'C' + name ;
    for(auto n : names) {
        if(name == n) {
            //return;
            //throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double value_double = Change_unitsValue(value_string);
    if (value_double <= 0) {
        throw Get_Exception::Invalid_Value_Exception();
    }
    names.push_back(name);
    Circuitcontroller.get_Capacitors(name , node1 , node2 , value_string , value_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << value_double <<"  Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << value_string << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_inductor(std::string name  , std::string node1 , std::string node2 , std::string value_string) {
    name = 'L' + name ;
    for(auto n : names) {
        if(name == n) {
            //throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double value_double = Change_unitsValue(value_string);
    if (value_double <= 0) {
        //return;
        throw Get_Exception::Invalid_Value_Exception();
    }
    names.push_back(name);
    Circuitcontroller.get_Inductor(name , node1 , node2 , value_string , value_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << value_double <<"  Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << value_string << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_Diod(std::string name  , std::string node1 , std::string node2 , std::string model) {
    name = 'D' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    if (model == "Z"  || model == "D") {}
    else {
        throw Get_Exception::Invalid_DiodModel_Exception(model);
    }
    names.push_back(name);
    Circuitcontroller.get_diode(name , node1 , node2 , model);
    std::cout << name << " "<< node1 << " " << node2 << " " << model <<"  Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << model << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_Ground(std::string node) {
    Circuitcontroller.get_Ground(node);
    std::cout << "Gruond " << node << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << node << " Became Ground" << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Delete_element(std::string name , char RLC) {
    name = RLC + name ;
    bool found = false;
    for (auto it = names.begin(); it != names.end(); ) {
        if (*it == name) {
            it = names.erase(it);
            found = true;
            std::cout << name  << " Deleted" << std::endl;
        }
        else {
            ++it;
        }
    }
    if (!found) {
        throw Get_Exception::Delete_Exception(RLC);
    }
    Circuitcontroller.get_Deleted_element(name , RLC);

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " Deleted" << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Delete_ground(std::string node) {
    std::cout << "Ground " << node << " deleted" << std::endl;
    Circuitcontroller.get_Ungrounded_node(node);

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << node << " IS Not Ground anymore" << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_VoltageSource(std::string name  , std::string node1 , std::string node2 , std::string value_string) {
    name = 'V' + name ;
    for(auto n : names) {
        if(name == n) {
            ///return;
            //throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double value_double = Change_unitsValue(value_string);
    if (value_double <= 0) {
        throw Get_Exception::Invalid_Value_Exception();
    }
    names.push_back(name);
    Circuitcontroller.get_Voltage_source(name , node1 , node2 , value_string , value_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << value_double <<" Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << value_string << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_CurrentSource(std::string name  , std::string node1 , std::string node2 , std::string value_string) {
    name = 'I' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double value_double = Change_unitsValue(value_string);
    if (value_double <= 0) {
        throw Get_Exception::Invalid_Value_Exception();
    }
    names.push_back(name);
    Circuitcontroller.get_Current_source(name , node1 , node2 , value_string , value_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << value_double <<"  Done" << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << value_string << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler:: handle_Add_AC_voltage(std::string name , std::string node1 , std::string node2 , std::string ampl , std::string phase) {
    name = 'V' + name ;
    double ampl_double = Change_unitsValue(ampl);
    double phase_double = Change_unitsValue(phase);
    Circuitcontroller.get_AC_voltage(name , node1 , node2 , ampl_double,phase_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << ampl <<"  Done" << std::endl;

}
void CommandHandler::handle_Add_SIN_voltage(std::string name , std::string node1 , std::string node2 , std::string offset , std::string ampl , std::string Freq,
    std::string delay,std::string theta,std::string phi,std::string cycles) {
    name = 'V' + name ;
    for(auto n : names) {
        if(name == n) {
           // throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double offset_double = Change_unitsValue(offset);
    double ampl_double = Change_unitsValue(ampl);
    double freq_double = Change_unitsValue(Freq);
    double delay_double = Change_unitsValue(delay);
    double theta_double = Change_unitsValue(theta);
    double phi_double = Change_unitsValue(phi);
    double cycles_double = Change_unitsValue(cycles);

    names.push_back(name);
    Circuitcontroller.get_SIN_voltage(name , node1 , node2 , offset_double,
        ampl_double, freq_double,delay_double, theta_double, phi_double, cycles_double);
    // std::cout << name << " " << node1 << " " << node2 << " " << Vofset_double << " " <<
    //     Vamp_double << " " << freq_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << "SIN " << name << " " << node1 << " " << node2 << " " << "(Vofset :" << Vofset << " /Vamp : " << Vamp << " /Freq : " << Freq << ")" << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_PULSE_voltage(std::string name , std::string node1 , std::string node2 ,
    std::string VInitial , std::string VOn , std::string TDelay,
    std::string TRise,std::string TFall,std::string TOn,std::string TPeriod ,std::string cycles) {
    name = 'V' + name ;
    for(auto n : names) {
        if(name == n) {
            // throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double initial_double = Change_unitsValue(VInitial);
    double Von_double = Change_unitsValue(VOn);
    double delay_double = Change_unitsValue(TDelay);
    double rise_double = Change_unitsValue(TRise);
    double fall_double = Change_unitsValue(TFall);
    double TOn_double = Change_unitsValue(TOn);
    double period_double = Change_unitsValue(TPeriod);
    double cycles_double = Change_unitsValue(cycles);

    names.push_back(name);
    Circuitcontroller.get_PULSE_voltage(name , node1 , node2 , initial_double,
        Von_double,delay_double, rise_double, fall_double,TOn_double,period_double, cycles_double);
    // std::cout << name << " " << node1 << " " << node2 << " " << Vofset_double << " " <<
    //     Vamp_double << " " << freq_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << "SIN " << name << " " << node1 << " " << node2 << " " << "(Vofset :" << Vofset << " /Vamp : " << Vamp << " /Freq : " << Freq << ")" << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_VCVS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 , std::string CtrN2 , std::string Gain) {
    name = 'E' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double Gain_double = Change_unitsValue(Gain);
    names.push_back(name);
    Circuitcontroller.get_VCVS(name , node1 , node2 , CtrN1 , CtrN2 ,Gain, Gain_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << Gain_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << CtrN1 << " " << CtrN2 << " " << Gain << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_VCCS(std::string name , std::string node1 , std::string node2 , std::string CtrN1 , std::string CtrN2 , std::string Gain) {
    name = 'G' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    double Gain_double = Change_unitsValue(Gain);
    names.push_back(name);
    Circuitcontroller.get_VCCS(name , node1 , node2 , CtrN1 , CtrN2 ,Gain, Gain_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << Gain_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << CtrN1 << " " << CtrN2 << " " << Gain << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_CCVS(std::string name , std::string node1 , std::string node2 , std::string Vname , std::string Gain){
    name = 'H' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    bool found = false;
    for(auto n : names) {
        if(Vname == n) {
            found = true;
        }
    }
    if(!found) {
        throw Get_Exception::Element_NameException(Vname);
    }
    double Gain_double = Change_unitsValue(Gain);
    names.push_back(name);
    Circuitcontroller.get_CCVS(name , node1 , node2 , Vname , Gain, Gain_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << Gain_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << Vname << " " << Gain << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_CCCS(std::string name , std::string node1 , std::string node2 , std::string Vname , std::string Gain){
    name = 'F' + name ;
    for(auto n : names) {
        if(name == n) {
            throw Get_Exception::Duplicate_Name_Exception(name);
        }
    }
    bool found = false;
    for(auto n : names) {
        if(Vname == n) {
            found = true;
        }
    }
    if(!found) {
        throw Get_Exception::Element_NameException(Vname);
    }
    double Gain_double = Change_unitsValue(Gain);
    names.push_back(name);
    Circuitcontroller.get_CCCS(name , node1 , node2 , Vname , Gain, Gain_double);
    std::cout << name << " "<< node1 << " " << node2 << " " << Gain_double << std::endl;

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << name << " " << node1 << " " << node2 << " " << Vname << " " << Gain << std::endl;
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Add_TRAN(std::string Tstep , std::string Tstop , std::string Tstart , std::string Tmaxstep) {
    double Tstep_double = Change_unitsValue(Tstep);
    double Tstop_double = Change_unitsValue(Tstop);
    double Tstart_double = Change_unitsValue(Tstart);
    double Tmax_double = Change_unitsValue(Tmaxstep);

    Circuitcontroller.get_TRAN(Tstep, Tstep_double ,Tstop,Tstop_double, Tstart,
        Tstart_double ,Tmaxstep ,Tmax_double);

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // File_output << "TRAN : " <<Tstep << " " << Tstop << " " << Tstart << " " << Tmaxstep  << std::endl;
    // Files.back().write(File_output.str());
}

////////////////////////////mir
void CommandHandler::handle_DC_Analysis() {
    std::cout << "DC Analysis..." << std::endl;
    Circuitcontroller.DC_solve();
    std::cout<<"done1\n";

}
void CommandHandler::handle_Tran_Analysis(std::string TStep,std::string TStop,std::string TStart,std::string TMax_step,std::string variables) {
    std::cout << "\nTran Analysis..." << std::endl;
    std::vector<std::string> names;
    std::string n;
    for(int i=0 ; i<variables.size() ; i++) {
        if(variables[i] == 'V' || variables[i]=='I') {
            n.push_back(variables[i]);
            for(int j=i+2 ; variables[j]!=')'; j++) {
                n.push_back(variables[j]);
            }
            names.push_back(n);
            n.clear();
        }
    }
    double Tstep_double= Change_unitsValue(TStep);
    double Tstop_double = Change_unitsValue(TStop);
    double Tstart_double = Change_unitsValue(TStart);
    double Tmax_double = Change_unitsValue(TMax_step);



    Circuitcontroller.tran_solve(Tstep_double,Tstop_double,Tstart_double,Tmax_double,names);
    ///////////////////// File Handling
    // std::ostringstream File_output;
    // for(int i=0 ; i<names.size() ; i++) {
    //     File_output << "TRAN : " <<TStep << " " << TStop << " " << TStart << " " << TMax_step  << " "<<names[i][0]<<"("<<names[i].substr(1)<<")"<< std::endl;
    // }
    // Files.back().write(File_output.str());

}
void CommandHandler::handle_AC_Analysis(std::string FStart,std::string FStop,std::string numOfPoints,std::string typeOfSweep,std::string variables,std::string outputType) {
    std::cout << "\nAC Analysis..." << std::endl;
    std::vector<std::string> names;
    std::string n;
    for(int i=0 ; i<variables.size() ; i++) {
        if(variables[i] == 'V' || variables[i]=='I') {
            n.push_back(variables[i]);
            for(int j=i+2 ; variables[j]!=')'; j++) {
                n.push_back(variables[j]);
            }
            names.push_back(n);
            n.clear();
        }
    }

    double num_int = Change_unitsValue(numOfPoints);
    double Fstart_double = Change_unitsValue(FStart);
    double Fstop_double = Change_unitsValue(FStop);

    //double Tmax_double = Change_unitsValue(TMax_step);

    //std::cout<<"start: "<<Fstart_double<<" stop: "<<Fstop_double<<" N: "<<num_int<<" type: "<<typeOfSweep<<" variables: "<<names[0]<<std::endl;


    Circuitcontroller.ac_solve(Fstart_double,Fstop_double ,num_int,typeOfSweep,names,outputType);

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // for(int i=0 ; i<names.size() ; i++) {
    //     File_output << "TRAN : " <<TStep << " " << TStop << " " << TStart << " " << TMax_step  << " "<<names[i][0]<<"("<<names[i].substr(1)<<")"<< std::endl;
    // }
    // Files.back().write(File_output.str());
}
void CommandHandler::handle_Phase_Analysis(std::string baseFreq,std::string PStart,std::string PStop,std::string numOfPoints,std::string variables) {
    std::cout << "\nPhase Analysis..." << std::endl;
    std::vector<std::string> names;
    std::string n;
    for(int i=0 ; i<variables.size() ; i++) {
        if(variables[i] == 'V' || variables[i]=='I') {
            n.push_back(variables[i]);
            for(int j=i+2 ; variables[j]!=')'; j++) {
                n.push_back(variables[j]);
            }
            names.push_back(n);
            n.clear();
        }
    }
    int num_int = Change_unitsValue(numOfPoints);
    double baseFdouble = Change_unitsValue(baseFreq);
    double Pstop_double = Change_unitsValue(PStop);
    double Pstart_double = Change_unitsValue(PStart);
    //double Tmax_double = Change_unitsValue(TMax_step);

    //std::cout<<"start: "<<Pstart_double<<" stop: "<<Pstop_double<<" N: "<<num_int<<" baseFreq: "<<baseFdouble<<" variables: "<<names[0]<<std::endl;

    Circuitcontroller.phase_solve(baseFdouble,Pstart_double,Pstop_double,num_int,names);

    ///////////////////// File Handling
    // std::ostringstream File_output;
    // for(int i=0 ; i<names.size() ; i++) {
    //     File_output << "TRAN : " <<TStep << " " << TStop << " " << TStart << " " << TMax_step  << " "<<names[i][0]<<"("<<names[i].substr(1)<<")"<< std::endl;
    // }
    // Files.back().write(File_output.str());

}
void CommandHandler::handle_print_all_nodes() {
    std::cout<<"printing all nodes..."<<std::endl;
    Circuitcontroller.print_All_nodes();
}
void CommandHandler::handle_print_All_elements() {
    std::cout<<"printing all elements..."<<std::endl;
    Circuitcontroller.print_All_elements();

}
void CommandHandler::handle_print_element(std::string type) {
    std::cout<<"printing all "<<type<<" elements..."<<std::endl;
    Circuitcontroller.print_element(type);
}
void CommandHandler::handle_change_node_name(std::string old_name, std::string new_name) {
    Circuitcontroller.change_node_name(old_name,new_name);
}



