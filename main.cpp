#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include "src/KeyValueServer.h"
#include <arpa/inet.h>

int main() {
    
    const int port = 8080;
    const std::string storage_file = "./database.dat";
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Failed to set socket options" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }
    
    // Start listening
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << port << std::endl;
    std::cout << "Storage file: " << storage_file << std::endl;

    KeyValueServer keyValServer(storage_file);

    std::vector<std::thread> client_threds;

    while (true) {
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, sizeof(client_ip));
        std::cout << "Accepted connection from " << client_ip << ":" << ntohs(address.sin_port) << std::endl;

        client_threds.emplace_back(&KeyValueServer::handleClient, &keyValServer, new_socket);
    }

    close(server_fd);

    for (auto &thread : client_threds) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Server shutting down." << std::endl;

    return 0;
}