
#include <iostream>
#include <string>
#include "Client.h"

int main(int argc, char *argv[]) {
    // Ensure the correct number of arguments are provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <port>" << std::endl;
        return 1;
    }

    try {
        // Parse command line arguments
        std::string serverAddress = argv[1];
        int port = std::stoi(argv[2]);

        Client client(serverAddress, port);
        client.start();  // Start client execution (connects to server, spawns threads)

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
