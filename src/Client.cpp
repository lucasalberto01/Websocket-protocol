

#include "Client.hpp"

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

Client::Client() {
    this->port = 0;
    this->server = nullptr;
    return;
}

Client::~Client() {
    delete this->server;
    return;
}

int Client::begin(string host, int port) {
    this->client_session = socket(AF_INET, SOCK_STREAM, 0);

    if (this->client_session < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    int status;

    do {
        status = connect(this->client_session, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        if (status < 0) {
            printf("Connection Failed - Retry \n");
            usleep(5 * 1000000);
        }
    } while (status < 0);

    this->status = ClientStatus::CONNECT;
    printf("Connect Success\n");

    return 0;
}

void Client::onMessage(void (*funcptr)(char*, int)) {
    this->t = thread(&Client::taskOnMessage, this, funcptr);
    return;
}

void Client::taskOnMessage(void (*funcptr)(char*, int)) {
    while (this->status == ClientStatus::CONNECT) {
        int valread = read(this->client_session, this->buffer, 1024 - 1);
        if (valread < 0) {
            printf("Error reading from socket\n");
        } else if (valread == 0) {
            // printf("Aguardando novos dados dados\n");
        } else {
            this->buffer[valread] = '\0';
            funcptr(this->buffer, valread);
            memset(this->buffer, 0, sizeof(this->buffer));
        }
    }
}

int Client::sender() {
    char* hello = "Hello from client";
    send(this->client_session, hello, strlen(hello), 0);
    return 0;
}

void Client::waitForMessage() {
    if (this->t.joinable()) {
        this->t.join();
    }
}