
#include "Client.h"

Client::Client(std::string address, int port) 
    : serverAddress(address), port(port), sockfd(-1), running(false) {}

Client::~Client() {
    disconnect();
}

// Establishes TCP connection to the STOMP server
bool Client::connectToServer() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
        return false;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Could not connect to server." << std::endl;
        return false;
    }

    running = true;
    return true;
}

// Sends STOMP frames to the server
void Client::sendMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(sendMutex);
    if (send(sockfd, message.c_str(), message.size(), 0) == -1) {
        std::cerr << "Error sending message to server." << std::endl;
    }
}

// Reads user input and executes commands
void Client::readUserInput() {
    std::string line;
    while (running) {
        std::getline(std::cin, line);
        if (line.empty()) continue;

        Command cmd(line, *this);
        cmd.execute();

        if (line == "logout") {  
            disconnect();
        }
    }
}

// Listens for incoming messages from the server
void Client::receiveMessages() {
    char buffer[1024];
    
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead <= 0) {
            std::cerr << "Server disconnected." << std::endl;
            running = false;
            break;
        }

        std::string response(buffer);
        stompProtocol.processServerFrame(response);
}


// Starts the client (connects and launches threads)
void Client::start() {
    if (!connectToServer()) {
        return;
    }

    std::cout << "Connected to server at " << serverAddress << ":" << port << std::endl;

    inputThread = std::thread(&Client::readUserInput, this);
    listenerThread = std::thread(&Client::receiveMessages, this);

    inputThread.join();
    listenerThread.join();
}

// Disconnects the client
void Client::disconnect() {
    if (running) {
        running = false;
        close(sockfd);
        std::cout << "Disconnected from server." << std::endl;
    }
}
