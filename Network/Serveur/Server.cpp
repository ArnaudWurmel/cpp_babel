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
                std::string body = "disc " + (*it)->getUsername();

                triggerEvent((*it)->getId(), body);
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
    if (taken || pseudo.size() >= babel::Message::maxBodySize) {
        user.sendResponse(Message::MessageType::Error, "KO");
    }
    else {
        user.setUsername(pseudo);
        user.sendResponse(Message::MessageType::Connect, pseudo);
        std::string     body = "con " + user.getUsername();
        triggerEvent(user.getId(), body);
    }
}

void    babel::Server::userList(babel::User& caller, babel::Message& message) {
    std::set<std::unique_ptr<babel::User> >::const_iterator it = _userList.begin();
    std::string res = "";

    while (it != _userList.end()) {
        if ((*it)->getUsername().size()) {
            res = res + (*it)->getUsername() + ";";
        }
        ++it;
    }
    caller.sendResponse(babel::Message::MessageType::Userlist, res);
}

void    babel::Server::triggerEvent(unsigned int senderId, std::string const& body) {
    std::set<std::unique_ptr<User> >::iterator  it = _userList.begin();

    while (it != _userList.end()) {
        if ((*it)->getId() != senderId) {
            (*it)->sendResponse(babel::Message::MessageType::Event, body);
        }
        ++it;
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
