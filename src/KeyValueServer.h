#include <iostream>
#include <cstring>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <fstream>
#include <unistd.h>
#include <sstream>

class KeyValueServer {
    private:
        std::unordered_map<std::string, std::string> storage;
        std::mutex data_mutex;
        std::string storage_file;

        void loadFromFile();
        void saveToFile();

    public:
        KeyValueServer(const std::string& filename);
        ~KeyValueServer();

        void handleClient(int client_socket);
        std::string handleRequest(const std::string& request);
};