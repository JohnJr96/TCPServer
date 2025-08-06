# TCPServer
This is a simple TCP Server that accepts key-value operations from multiple clients.

Files:
main.cpp - The main.cpp file handles the socket creation for the server.
src/KeyValueServer.cpp - This file handles the client connections, requests, updates the stored key-value data and send response to the requests.
testCases.py - This file contains a basic test script to simulate 4 clients concurrently, sending requests and printing responses.
database.dat - This file stores the key-value pairs received through the requests, so that the data is available after server shut down.

Execution steps:
In the project folder
>cmake .
>make

This will generate the executable "TCPServer"

>./TCPServer

This command starts the server and the service would be ready to accept requests from the clients

Test steps:
>python3 testCases.py

This command runs a basic test script with 4 clients, sending SET, GET, DEL and an invalid request and prints the output of each operation.

Manual testing:
In a terminal
>telnet localhost 8080

send commands:

Accepted commands are-

SET <key> <value> -> stores the key value pairs and send response "OK"
GET <key> -> returns the value of the specified key
DELETE <key> -> delete the key-value pair of the specified key
LIST -> returns all the stored key value pairs

