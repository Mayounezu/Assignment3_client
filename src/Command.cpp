#include "../include/Command.h"
#include <sstream>
#include "../include/FrameUtil.h"
#include "../include/StompProtocol.h"

using namespace std;

Command::Command(string line, StompProtocol stompProtocol){
    std::stringstream ss(line);

    while(ss.eof()){
        string word;
        ss << word;
        commandArgs.push_back(word);
    }

    commandName = commandArgs[0];
    commandArgs.erase(commandArgs.begin());

    Command::stompProtocol = stompProtocol;
}

std::map<std::string, std::string> Command::handleLoginCommand(){
  bool canLogin = true;
  try{
    checkLogin();
    canLogin = true;
    }catch(const std::exception& e){
        canLogin = false;
        std::cerr << e.what() << std::endl;
    }

    if(canLogin){
        std::map<std::string, std::string> loginMap;
        loginMap["accept-version"] = FrameUtil::acceptVersion;
        loginMap["host"] = FrameUtil::host;
        loginMap["login"] = commandArgs[1];
        loginMap["passcode"] = commandArgs[2];

        Command::stompProtocol::handler = new ConnectionHandle(commandArgs[0], 80);

        Command::stompProtocol.sendFrame("CONNECT", loginMap, "");
    }
}

std::map<std::string, std::string> Command::handleJoinCommand(){
    bool canJoin = true;
    try{
        checkJoin();
        canJoin = true;
    }catch(const std::exception& e){
        canJoin = false;
        std::cerr << e.what() << std::endl;
    }

    if(canJoin){
        std::map<std::string, std::string> joinMap;
        joinMap["destination"] = commandArgs[0];
        joinMap["id"] = commandArgs[1];
        Command::stompProtocol.sendFrame("SUBSCRIBE", joinMap, "");
    }
}




