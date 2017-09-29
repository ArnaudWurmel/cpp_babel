//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include <functional>
#include "Server.h"

babel::Server::Server(unsigned int port) : Logger("Server") {
    Logger::say("Instancied");
    _threadRunning = true;
    _socketAcceptor = std::unique_ptr<ISocketAcceptor>(new babel::BoostAcceptor(port, _haveAction, _cv));
    _acceptorThread = std::unique_ptr<std::thread>(new std::thread(&babel::Server::threadLoop, this));
    _socketAcceptor->run();
}

void    babel::Server::threadLoop() {
    while (_threadRunning) {
        std::unique_lock<std::mutex>    lck(this->_haveAction);
        _cv.wait(lck);
        while (_socketAcceptor->haveAWaitingClient()) {
            std::shared_ptr<ISocket>    newSocket = _socketAcceptor->acceptClient();
            babel::User *user = new babel::User(newSocket);

            user->initNetwork();
            _userList.insert(std::unique_ptr<User>(user));
        }
        std::set<std::unique_ptr<User> >::iterator  it = _userList.begin();

        while (it != _userList.end()) {
            if (!(*it)->manageData()) {
                _userList.erase(it);
                say("Client ended");
            }
            ++it;
        }
    }
}

babel::Server::~Server() {
    _acceptorThread->join();
    _userList.clear();
    _socketAcceptor->stop();
    _socketAcceptor.reset();
}
