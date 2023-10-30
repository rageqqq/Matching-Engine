# Matching-Engine

To compile the client code:
g++ -std=c++11 .\client.cpp -lws2_32 -o client

To compile the server code:
g++ -std=c++11 .\server.cpp -lws2_32 -o server

To run the executables:
server.exe 
client.exe [ip-address] (localhost in this case) (eg. client.exe localhost)

To compile and run the testing code:
g++ -std=c++11 .\test.cpp -o test
test.exe
