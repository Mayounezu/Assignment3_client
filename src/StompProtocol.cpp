#include "../include/StompProtocol.h"
#include <sstream>
#include <iostream>
#include <map>

StompProtocol::StompProtocol(ConnectionHandler& handler) : handler(handler) {}

std::string StompProtocol::createFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::ostringstream frame;
    frame << command << "\n";
    for (const auto& header : headers) {
        frame << header.first << ":" << header.second << "\n";
    }
    frame << "\n" << body << "\0";
    return frame.str();
}

void StompProtocol::processServerFrame(const std::string& frame) {
    std::istringstream stream(frame);
    std::string command;
    std::getline(stream, command);

    if (command == "CONNECTED") {
        std::cout << "Connection successful." << std::endl;
    }
    else if (command == "MESSAGE") {
        handleMessageFrame(stream);
    }
    else if (command == "RECEIPT") {
        handleReceiptFrame(stream);
    }
    else if (command == "ERROR") {
        handleErrorFrame(stream);
    }
}

void StompProtocol::handleMessageFrame(std::istringstream& stream) {
    std::cout << "Message received from server:" << std::endl;
    std::string line;
    while (std::getline(stream, line)) {
        std::cout << line << std::endl;
    }
}

void StompProtocol::handleReceiptFrame(std::istringstream& stream) {
    std::cout << "Receipt acknowledged by server." << std::endl;
}

void StompProtocol::handleErrorFrame(std::istringstream& stream) {
    std::cerr << "Error frame received from server:" << std::endl;
    std::string line;
    while (std::getline(stream, line)) {
        std::cerr << line << std::endl;
    }
}

void StompProtocol::sendFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::string frame = createFrame(command, headers, body);
    handler.sendFrameAscii(frame, '\0');
}
