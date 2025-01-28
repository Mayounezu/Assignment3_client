#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "StompProtocol.h"
class Command{

private:
    string std::commandName;
    std::vector<string> commandArgs;
    StompProtocol stompProtocol;

    handleLoginCommand();
    handleJoinCommand();
    handleExitCommand();
    handleReportCommand();
    handleSummaryCommand();
    handleLogoutCommand();

public:
    Command(string line){


      }
    std::map<std::string, std::string> processCommand(){
        std::map<std::string, std::string> commandMap;
        commandMap.insert(std::pair<std::string, std::string>("commandName", commandName));
        for(int i = 0; i < commandArgs.size(); i++){
            commandMap.insert(std::pair<std::string, std::string>("arg" + std::to_string(i), commandArgs[i]));
        }
        return commandMap;

    }






    



}