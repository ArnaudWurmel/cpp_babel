//
// Created by wurmel_a on 26/09/17.
//

#include <iostream>
#include "User.h"

babel::User::User(std::shared_ptr<ISocket> socket) : _socket(socket) {
    _continue = true;
}

void    babel::User::setUsername(std::string const& username) {
    _username = username;
}

std::string const& babel::User::getUsername() const {
    return _username;
}

void    babel::User::setIpAddr(std::string const& ipAddr) {
    _ipAddr = ipAddr;
}

std::string const&    babel::User::getIpAddr() const {
    return _ipAddr;
}

void    babel::User::initNetwork() {
    try {
        setIpAddr(_socket->getIpAddr());
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

bool    babel::User::manageData() {
    return _continue;
}

babel::User::~User() {
    _socket.reset();
}