//
// Created by wurmel_a on 26/09/17.
//

#include <iostream>
#include <utility>
#include "Server.h"
#include "User.h"

unsigned int babel::User::_userId = 0;

babel::User::User(std::shared_ptr<ISocket> socket, babel::Server& server) : Logger("User"), _socket(socket), _server(server) {
    _continue = true;
    _functionPtrs.insert(std::make_pair(Message::MessageType::Connect, &babel::Server::connectUser));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Userlist, &babel::Server::userList));
    _functionPtrs.insert(std::make_pair(Message::MessageType::ChannelList, &babel::Server::channelList));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Join, &babel::Server::joinChannel));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Leave, &babel::Server::leaveChannelFor));
    _functionPtrs.insert(std::make_pair(Message::MessageType::CreateChannel, &babel::Server::createChannel));
    _id = ++babel::User::_userId;
}

void    babel::User::setUsername(std::string const& username) {
    _username = username;
}

std::string const& babel::User::getUsername() const {
    return _username;
}

unsigned int    babel::User::getId() const {
    return _id;
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
        _socket->close();
    }
}

bool    babel::User::manageData() {
    while (_socket->haveAvailableData()) {
        babel::Message  message = _socket->getAvailableMessage();

        if (_functionPtrs.find(message.getType()) != _functionPtrs.end()) {
            (_server.*_functionPtrs[message.getType()])(*this, message);
        }
    }
    return _socket->isOpen();
}

void    babel::User::sendResponse(babel::Message::MessageType mType, std::string const& body) {
    Message message(mType);

    message.setBody(body.c_str(), body.size());
    std::cout << "Body : " << body << std::endl;
    _socket->write(message);
}

void    babel::User::leaveChannel() {
    _server.leaveChannel(*this);
}

babel::User::~User() {
    _socket.reset();
    say("Deleted");
}