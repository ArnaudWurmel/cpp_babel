//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Server.h"

babel::Server::Server(unsigned int port) : Logger("Server") {
    Logger::say("Instancied");
    _socketAcceptor = std::unique_ptr<ISocketAcceptor>(new babel::BoostAcceptor(port));
    _socketAcceptor->run();
    start_accept();
}

void    babel::Server::start_accept() {

}

babel::Server::~Server() {}
