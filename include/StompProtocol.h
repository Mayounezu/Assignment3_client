#pragma once

#include "../include/ConnectionHandler.h"
#include <string>
#include <map>
#include <sstream>

class StompProtocol {
private:
    ConnectionHandler& handler;

    // Helper methods for processing frames
    void handleMessageFrame(std::istringstream& stream);
    void handleReceiptFrame(std::istringstream& stream);
    void handleErrorFrame(std::istringstream& stream);

    // Helper to construct a STOMP frame
    std::string createFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body);

public:
    // Constructor
    explicit StompProtocol(ConnectionHandler& handler);

    // Process an incoming frame
    void processServerFrame(const std::string& frame);

    // Send a frame to the server
    void sendFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body);
};
