#pragma once

#include <netinet/in.h>

#include <thread>

class Server {
   public:
    Server();
    ~Server();
    void start(int port);
    void waitForMessage();
    void onMessage(void (*funcptr)(char*, int));

   private:
    // Attributes
    int slots_total = 3;
    int slots[3] = {0};
    int slots_index = 0;
    std::thread loopMsg;
    std::thread loopAccept;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // Methods
    void taskOnMessage(void (*funcptr)(char*, int));
    void startAccept(int server_fd);
    void handlerAccept(int server_fd);
    int createSocket();
    void setSocketOptions(int server_fd, int opt);
    void bindSocketToPort(int server_fd, int port);
    void startListening(int server_fd);
    int findFreeSlot();
};