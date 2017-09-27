//
// Created by wurmel_a on 26/09/17.
//

#include "User.h"

babel::User::User(std::shared_ptr<ISocket> socket) : _socket(socket) {}

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

babel::User::~User() {}