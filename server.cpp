#include "src/Server.hpp"

#include <stdio.h>

void onMessage(char* buffer, int size) {
    printf("Message: %s\n", buffer);
    return;
}

int main(int argc, char const* argv[]) {
    Server server;

    server.start(8080);
    server.onMessage(onMessage);

    server.waitForMessage();

    printf("Hello World\n");

    return 0;
}