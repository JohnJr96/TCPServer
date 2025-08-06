#include "KeyValueServer.h"
#include <algorithm>
#include <vector>

KeyValueServer::KeyValueServer(const std::string &filename) : storage_file(filename) {
    loadFromFile();
}

KeyValueServer::~KeyValueServer() {
    saveToFile();
}

void KeyValueServer::loadFromFile() {
    std::ifstream file(storage_file);
    if (!file.is_open()) {
        std::cerr << "Failed to open storage file: " << storage_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t seperator = line.find('=');
        if (seperator != std::string::npos) {
            std::string key = line.substr(0, seperator);
            std::string value = line.substr(seperator + 1);
            storage[key] = value;
        }
    }
    file.close();
    std::cout << "Loaded data from file." << std::endl;
}

void KeyValueServer::saveToFile() {
    std::cout << "Saving changes to file." << std::endl;
    std::ofstream file(storage_file);
    if (!file.is_open()) {
        std::cerr << "Failed to open storage file for writing: " << storage_file << std::endl;
        return;
    }

    for (const auto& pair : storage) {
        file << pair.first << "=" << pair.second << std::endl;
    }
    file.close();
    std::cout << "Data saved to file." << std::endl;
}

void KeyValueServer::handleClient(int client_socket) {
    char buffer[1024] = {0};

    while (true) {
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cerr << "Client disconnected or error occurred." << std::endl;
            break;
        }

        std::string request(buffer, bytes_read);

        //remove trailing newline
        if (!request.empty() && request.back() == '\n') {
            request.pop_back();
        }

        std::string response = handleRequest(request) + "\n";

        write(client_socket, response.c_str(), response.size());
        
        memset(buffer, 0, sizeof(buffer));
    }
}

std::string KeyValueServer::handleRequest(const std::string &request)
{
    std::vector<std::string> tokens;

    std::istringstream iss(request);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.empty()) {
        return "Error: Empty request";
    }

    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    std::lock_guard<std::mutex> lock(data_mutex);

    if (command == "GET") {
        if (tokens.size() != 2) {
            return "Error: GET command requires a key";
        }
        const std::string& key = tokens[1];
        auto it = storage.find(key);
        if (it != storage.end()) {
            return "Value " + it->second;
        }
        else {
            return "Error: NOT FOUND";
        }
    }

    else if (command == "SET") {
        if (tokens.size() != 3) {
            return "Error: SET command requires a key and a value";
        }
        const std::string& key = tokens[1];
        const std::string& value = tokens[2];
        storage[key] = value;
        saveToFile();
        return "OK";
    }

    else if (command == "DELETE") {
        if (tokens.size() != 2) {
            return "Error: DELETE command requires a key";
        }
        const std::string& key = tokens[1];
        auto it = storage.find(key);
        if (it != storage.end()) {
            storage.erase(it);
            saveToFile();
            return "OK";
        }
        else {
            return "Error: NOT FOUND";
        }
    }

    else if (command == "LIST") {
        std::string response;
        for (const auto& pair : storage) {
            response += pair.first + "=" + pair.second + "\n";
        }
        return response.empty() ? "No entries found" : response;
    }

    return "Error: Unknown command";
}
