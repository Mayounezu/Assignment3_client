#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "ConnectionHandler.h"
#include "event.h"

class StompProtocol {
private:
    ConnectionHandler& handler;
    std::vector<Event> receivedEvents;  // Stores received events
    bool loggedIn;  // Tracks if user is logged in

    void handleMessageFrame(std::istringstream& stream);
    void handleReceiptFrame(std::istringstream& stream);
    void handleErrorFrame(std::istringstream& stream);

public:
    StompProtocol(ConnectionHandler& handler);
    
    void processServerFrame(const std::string& frame);
    void sendFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body);
    
    bool isLoggedIn() const;  
    const std::vector<Event>& getReceivedEvents() const;  
};
