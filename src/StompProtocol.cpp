#include "../include/StompProtocol.h"
#include <sstream>
#include <map>

StompProtocol::StompProtocol(ConnectionHandler& handler) : handler(handler), loggedIn(false) {}

bool StompProtocol::isLoggedIn() const {
    return loggedIn;
}

// Creates a properly formatted STOMP frame
std::string StompProtocol::createFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::ostringstream frame;
    frame << command << "\n";
    for (const auto& header : headers) {
        frame << header.first << ":" << header.second << "\n";
    }
    frame << "\n" << body << "\0";
    return frame.str();
}

// Processes incoming STOMP frames from the server
void StompProtocol::processServerFrame(const std::string& frame) {
    std::istringstream stream(frame);
    std::string command;
    std::getline(stream, command);

    if (command == "CONNECTED") {
        loggedIn = true;
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

// Handles incoming MESSAGE frames (event notifications)
void StompProtocol::handleMessageFrame(std::istringstream& stream) {
    std::string messageBody;
    std::string line;

    while (std::getline(stream, line)) {
        messageBody += line + "\n";  // Construct the full message body
    }

    // Convert STOMP message body into an Event object
    Event receivedEvent(messageBody);

    // Store the event for later use (summary generation, filtering, etc.)
    receivedEvents.push_back(receivedEvent);
}

// Handles RECEIPT frames (acknowledgment of sent frames)
void StompProtocol::handleReceiptFrame(std::istringstream& stream) {
    std::string receiptId;
    std::getline(stream, receiptId);

    if (receiptId.find("logout-receipt") != std::string::npos) {
        loggedIn = false;
        handler.close();  // Properly close connection
        std::cout << "Logout successful" << std::endl;
        exit(0);  // Only exit after logout receipt is received
    }
}

// Handles ERROR frames
void StompProtocol::handleErrorFrame(std::istringstream& stream) {
    std::string errorMessage;
    std::string line;
    
    while (std::getline(stream, line)) {
        errorMessage += line + "\n";
    }

    std::cerr << "STOMP Error: " << errorMessage << std::endl;
}

// Sends a STOMP frame to the server
void StompProtocol::sendFrame(const std::string& command, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::string frame = StompProtocol::createFrame(command, headers, body);
    handler.sendFrameAscii(frame, '\0');
}

// Getter for received events
const std::vector<Event>& StompProtocol::getReceivedEvents() const {
    return receivedEvents;
}
