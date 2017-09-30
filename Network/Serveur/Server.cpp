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
            babel::User *user = new babel::User(newSocket, *this);

            user->initNetwork();
            _userList.insert(std::unique_ptr<User>(user));
        }
        std::set<std::unique_ptr<User> >::iterator  it = _userList.begin();

        while (it != _userList.end()) {
            if (!(*it)->manageData()) {
                _userList.erase(it++);
                say("Client ended");
            }
            else
                ++it;
        }
    }
}

void    babel::Server::connectUser(babel::User& user, babel::Message& message) {
    std::string pseudo;

    if (message.getBodySize() == 0) {
        pseudo = "Invité #" + std::to_string(user.getId());
    }
    else {
        pseudo = std::string(message.getBody(), message.getBodySize());
    }
   std::set<std::unique_ptr<User> >::const_iterator    it = _userList.begin();
    bool taken = false;

    while (!taken && it != _userList.end()) {
        if ((*it)->getUsername().compare(pseudo) == 0) {
            taken = true;
        }
        ++it;
    }
    if (taken) {
        user.sendResponse(Message::MessageType::Error, "KO");
    }
    else {
        user.sendResponse(Message::MessageType::Connect, pseudo);
    }
}

babel::Server::~Server() {
    _cv.notify_one();
    _acceptorThread->join();
    _acceptorThread.reset();
    _userList.clear();
    _socketAcceptor->stop();
    _socketAcceptor.reset();
}
