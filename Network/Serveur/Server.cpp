//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Server.h"

babel::Server::Server(unsigned int port) : Logger("Server") {
    Logger::say("Instancied");
    _threadRunning = true;
    _socketAcceptor = std::unique_ptr<ISocketAcceptor>(new babel::BoostAcceptor(port, _haveAction, _cv));
    _acceptorThread = std::unique_ptr<std::thread>(new std::thread(&babel::Server::threadLoop, this));
    _socketAcceptor->run();
    _threadRunning = false;
}

void    babel::Server::threadLoop() {
    while (_threadRunning) {
        std::cout << "Called" << std::endl;
        std::unique_lock<std::mutex>    lck(this->_haveAction);
        _cv.wait(lck);

    }
}

babel::Server::~Server() {
    _acceptorThread->join();
}
