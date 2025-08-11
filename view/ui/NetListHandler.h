//
// Created by Purple-Rose on 7/26/2025.
//

#ifndef NETLISTHANDLER_H
#define NETLISTHANDLER_H

#include<string>
#include<iostream>
#include <vector>
#include <regex>

#include"CommandHandler.h"
class NetListHandler {
private:
    std::vector<std::string> netList;
    CommandHandler commandHandler;
    void handleNetList();
    public:
    void setNetList(std::vector<std::string> netList);
    void transientHandler (std::string dt , std::string TStop , std::string TStart,std::string variables);
    void acSweepHandler(std::string startFreq , std::string stopFreq , std::string n,std::string type ,std::string variables);
    void phaseSweepHandler(std::string baseFreq , std::string startPhase , std::string stopPhase,std::string n ,std::string variables);

};



#endif //NETLISTHANDLER_H
