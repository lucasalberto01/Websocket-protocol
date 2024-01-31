#pragma once

#include <string.h>

#include <string>
#include <thread>

using namespace std;

enum ClientStatus { DISCONNECT = 0, CONNECT = 1 };

class Client {
   public:
    Client();
    ~Client();
    int begin(string host, int port);
    int sender();
    void onMessage(void (*funcptr)(char*, int));
    void taskOnMessage(void (*funcptr)(char*, int));
    void waitForMessage();

   private:
    ClientStatus status = ClientStatus::DISCONNECT;
    int client_session;
    char buffer[1024] = {0};
    int port;
    char* server;
    thread t;
};