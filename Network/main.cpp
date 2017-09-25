#include <iostream>
#include "Serveur/Server.h"

int main() {
    babel::Server   server;

    if (server.initialize()) {
        return 0;
    }
    return 1;
}