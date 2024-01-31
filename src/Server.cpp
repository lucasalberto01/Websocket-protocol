
#include "Server.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

Server::Server() {}

Server::~Server() {}

void Server::start(int port) {
    int server_fd;
    int opt = 1;

    std::string hello = "Hello from server";

    server_fd = createSocket();
    setSocketOptions(server_fd, opt);
    bindSocketToPort(server_fd, port);
    startListening(server_fd);
    handlerAccept(server_fd);
}

int Server::createSocket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void Server::setSocketOptions(int server_fd, int opt) {
    if (setsockopt(server_fd, SOL_SOCKET, SO_PRIORITY, &opt, sizeof(opt))) {
        std::cerr << "Failed to set socket options" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocketToPort(int server_fd, int port) {
    // socklen_t addrlen = sizeof(address);

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind socket to port" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::startListening(int server_fd) {
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Failed to start listening" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::handlerAccept(int server_fd) {
    this->loopAccept = std::thread(&Server::startAccept, this, server_fd);
    return;
}

void Server::startAccept(int server_fd) {
    int addrlen = sizeof(this->address);

    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr*)&this->address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            exit(EXIT_FAILURE);
        } else {
            // Check if there is a free slot
            bool found = false;

            for (int i = 0; i < slots_total; i++) {
                if (slots[i] == 0) {
                    slots[i] = new_socket;
                    found = true;
                    printf("Client %d connected\n", i);
                    slots_index++;
                    break;
                }
            }

            if (!found) {
                printf("No free slots\n");
                close(new_socket);
            }
        }
    }
}

void Server::onMessage(void (*funcptr)(char*, int)) {
    this->loopMsg = std::thread(&Server::taskOnMessage, this, funcptr);
    return;
}

void Server::taskOnMessage(void (*funcptr)(char*, int)) {
    while (true) {
        for (int i = 0; i < slots_total; i++) {
            int session = slots[i];

            if (session == 0) {
                continue;
            }

            // Configurar o socket para o modo não bloqueante
            int flags = fcntl(session, F_GETFL, 0);
            fcntl(session, F_SETFL, flags | O_NONBLOCK);

            int valread = read(session, this->buffer, 1024 - 1);

            if (valread < 0) {
                // printf("Error reading from socket\n");
                continue;

            } else if (valread == 0) {
                close(session);
                printf("Client %d disconnected\n\n", i);
                slots[i] = 0;
                slots_index--;

            } else {
                this->buffer[valread] = '\0';
                funcptr(this->buffer, valread);
                memset(this->buffer, 0, sizeof(this->buffer));
            }
        }
    }
}

void Server::waitForMessage() {
    if (this->loopMsg.joinable()) {
        this->loopMsg.join();
    }

    if (this->loopAccept.joinable()) {
        this->loopAccept.join();
    }
}