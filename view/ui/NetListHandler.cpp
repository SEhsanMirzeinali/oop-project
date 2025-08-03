//
// Created by Purple-Rose on 7/26/2025.
//

#include "NetListHandler.h"



void NetListHandler:: setNetList(std::vector<std::string> netList) {
    this->netList = netList;
    handleNetList();
}
void NetListHandler::handleNetList() {
    std::regex addR(R"(R(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");
    std::regex addV(R"(V(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");
    std::regex addC(R"(C(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");
    std::regex addL(R"(L(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");
    std::regex addSin(R"(V(\S+)\s+(\S+)\s+(\S+)\s+SINE\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");
    std::regex addPulse(R"(V(\S+)\s+(\S+)\s+(\S+)\s+PULSE\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+))");

    std::smatch matches;
for(int i=0 ; i<netList.size() ; i++) {
    if(regex_match(netList[i], matches, addR)) {
        commandHandler.handle_Add_resistance(matches[1],matches[2],matches[3],matches[4]);
    }
    if(regex_match(netList[i], matches, addV)) {
        commandHandler.handle_Add_VoltageSource(matches[1],matches[2],matches[3],matches[4]);
    }
    if(regex_match(netList[i], matches, addC)) {
        commandHandler.handle_Add_capacitor(matches[1],matches[2],matches[3],matches[4]);
    }
    if(regex_match(netList[i], matches, addL)) {
        commandHandler.handle_Add_inductor(matches[1],matches[2],matches[3],matches[4]);
    }
    if(std::regex_match(netList[i],matches,addSin)) {
        commandHandler.handle_Add_SIN_voltage(matches[1],matches[2],matches[3],
            matches[4],matches[5],matches[6],matches[7],matches[8],matches[9],matches[10]);
    }
    if(std::regex_match(netList[i],matches,addPulse)) {
        commandHandler.handle_Add_PULSE_voltage(matches[1],matches[2],matches[3],
            matches[4],matches[5],matches[6],matches[7],matches[8],matches[9],matches[10],matches[11]);
    }
}
    commandHandler.handle_Add_Ground("0");
   // commandHandler.handle_Tran_Analysis("0.1","1","0","0","V(n001)");
    //commandHandler.handle_DC_Analysis();
}
void NetListHandler:: transientHandler(std::string dt , std::string TStop , std::string TStart,std::string variables) {
    commandHandler.handle_Tran_Analysis(dt,TStop,TStart,"0",variables);

}