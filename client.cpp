#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "src/Client.hpp"

#define PORT 8080
#define HOST "127.0.0.1"

void teste(char* message, int size) {
    printf("O tamanho da mensagem recebida é: %d\n", size);
    printf("A mensagem recebida é: %s\n", message);
    return;
}

int main(int argc, char const* argv[]) {
    Client connection;

    connection.begin(HOST, PORT);
    connection.onMessage(teste);

    connection.sender();

    connection.waitForMessage();

    return 0;
}

int mainBKP(int argc, char const* argv[]) {
    int status, valread, client_fd;
    char* hello = "Hello from client";
    char buffer[1024] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (status < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    valread = read(client_fd, buffer, 1024 - 1);
    if (valread < 0) {
        printf("Error reading from socket\n");
        return -1;
    }

    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}