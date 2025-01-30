#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "json.hpp"
#include "StompProtocol.h"
#include "Subscriptions.h"
#include "event.h" 

using json = nlohmann::json;

class Command {
private:
    std::string commandName;
    std::vector<std::string> commandArgs;
    StompProtocol& stompProtocol;
    Subscriptions subscriptions;

    // Private methods for handling commands
    void handleLoginCommand();
    void handleJoinCommand();
    void handleExitCommand();
    void handleReportCommand();
    void handleSummaryCommand();
    void handleLogoutCommand();

public:
    Command(const std::string& line, StompProtocol& protocol, Subscriptions& subscriptions);
    void execute();
};
