#include "Command.h"
#include "event.h"
#include "StompProtocol.h"

Command::Command(const std::string& line, StompProtocol& protocol, Subscriptions& subscriptions) 
    : stompProtocol(protocol), subscriptions(subscriptions) {
    std::istringstream iss(line);
    iss >> commandName;
    std::string arg;
    while (iss >> arg) {
        commandArgs.push_back(arg);
    }
}

void Command::execute() {
    static const std::map<std::string, void (Command::*)()> commandMap = {
        {"login", &Command::handleLoginCommand},
        {"join", &Command::handleJoinCommand},
        {"exit", &Command::handleExitCommand},
        {"report", &Command::handleReportCommand},
        {"summary", &Command::handleSummaryCommand},
        {"logout", &Command::handleLogoutCommand}
    };

    auto it = commandMap.find(commandName);
    if (it != commandMap.end()) {
        (this->*(it->second))();
    } else {
        std::cerr << "Unknown command: " << commandName << std::endl;
    }
}

void Command::handleLoginCommand() {
    if (commandArgs.size() < 3) {
        std::cerr << "Usage: login {host:port} {username} {password}" << std::endl;
        return;
    }

    std::map<std::string, std::string> headers = {
        {"accept-version", "1.2"},
        {"host", "stomp.cs.bgu.ac.il"},
        {"login", commandArgs[1]},
        {"passcode", commandArgs[2]}
    };

    stompProtocol.sendFrame("CONNECT", headers, "");
}

void Command::handleJoinCommand() {
    if (commandArgs.empty()) {
        std::cerr << "Usage: join {channel_name}" << std::endl;
        return;
    }

    std::string channel = commandArgs[0];

    if (!subscriptions.addSubscription(channel)) {
        std::cerr << "Already subscribed to " << channel << std::endl;
        return;
    }

    std::map<std::string, std::string> headers = {
        {"destination", "/" + channel},
        {"id", subscriptions.getSubscriptionId(channel)},
        {"receipt", "join-receipt"}
    };

    stompProtocol.sendFrame("SUBSCRIBE", headers, "");
}

void Command::handleExitCommand() {
    if (commandArgs.empty()) {
        std::cerr << "Usage: exit {channel_name}" << std::endl;
        return;
    }

    std::string channel = commandArgs[0];

    if (!subscriptions.removeSubscription(channel)) {
        std::cerr << "Not subscribed to " << channel << std::endl;
        return;
    }

    std::map<std::string, std::string> headers = {
        {"id", subscriptions.getSubscriptionId(channel)},
        {"receipt", "exit-receipt"}
    };

    stompProtocol.sendFrame("UNSUBSCRIBE", headers, "");
}

void Command::handleLogoutCommand() {
    std::map<std::string, std::string> headers = {
        {"receipt", "logout-receipt"}
    };

    stompProtocol.sendFrame("DISCONNECT", headers, "");
}

void Command::handleReportCommand() {
    if (commandArgs.empty()) {
        std::cerr << "Usage: report {file}" << std::endl;
        return;
    }

    std::string filename = commandArgs[0];

    names_and_events parsedData = parseEventsFile(filename);
    
    for (const auto& event : parsedData.events) {
        std::map<std::string, std::string> headers = {
            {"destination", "/" + parsedData.channel_name},
            {"user", "logged-in-user"}
        };

        stompProtocol.sendFrame("SEND", headers, event.toString());
    }
}

std::string generateSummary(const std::vector<Event>& userEvents, const std::string& channelName) {
    std::ostringstream summary;
    
    summary << "Channel " << channelName << "\n"
            << "Total Reports: " << userEvents.size() << "\n"
            << "Event Reports:\n";

    for (size_t i = 0; i < userEvents.size(); i++) {
        summary << "Report_" << (i + 1) << ":\n"
                << "City: " << userEvents[i].get_city() << "\n"
                << "Date: " << userEvents[i].get_date_time() << "\n"
                << "Event Name: " << userEvents[i].get_name() << "\n"
                << "Summary: " << userEvents[i].get_description().substr(0, 27) + "...\n";
    }

    return summary.str();
}

void Command::handleSummaryCommand() {
    if (commandArgs.size() < 3) {
        std::cerr << "Usage: summary {channel_name} {user} {file}" << std::endl;
        return;
    }

    std::string channelName = commandArgs[0];
    std::string username = commandArgs[1];
    std::string filename = commandArgs[2];

    const std::vector<Event>& receivedEvents = stompProtocol.getReceivedEvents();
    std::vector<Event> userEvents;

    for (const auto& event : receivedEvents) {
        if (event.get_channel_name() == channelName && event.getEventOwnerUser() == username) {
            userEvents.push_back(event);
        }
    }

    if (userEvents.empty()) {
        std::cerr << "No matching events found for user '" << username << "' in channel '" << channelName << "'." << std::endl;
        return;
    }

    std::sort(userEvents.begin(), userEvents.end(), [](const Event &a, const Event &b) {
        return a.get_date_time() < b.get_date_time();
    });

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    outFile << generateSummary(userEvents, channelName);
    outFile.close();
}
