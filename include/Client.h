#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Command.h"

class Client {
private:
    std::string serverAddress;
    int port;
    int sockfd;                 // Socket description tool
    std::thread inputThread;
    std::thread listenerThread;
    std::mutex sendMutex;        // Ensures thread safety when sending messages
    std::atomic<bool> running;   // Tracks if the client is still running
    std::queue<std::string> messageQueue;
    std::condition_variable cv;

    void readUserInput();         // Reads commands from std::cin
    void receiveMessages();       // Reads responses from server

public:
    Client(std::string address, int port);
    ~Client();

    bool connectToServer();
    void sendMessage(const std::string& message);
    void disconnect();
    void start();
};
