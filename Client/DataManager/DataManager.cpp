//
// Created by wurmel_a on 01/10/17.
//

#include "DataManager.h"
#include "../Socket/QtSocket.h"
#include "../Exceptions/Exception.h"

babel::DataManager::DataManager(std::string const& host, unsigned short port) : Logger("DataManager") {
    _functionPtrs.insert(std::make_pair(Message::MessageType::Connect, &babel::DataManager::handleConnect));
    say("Using : " + host + ":" + std::to_string(port));
    _socket = std::unique_ptr<ISocket>(new babel::QtSocket(_locker, _cv));
    if (!_socket->connectSocket(host, port)) {
        throw NetworkException();
    }
    _socket->startSession();
}

bool    babel::DataManager::connectUser(std::string const& name) {
    bool    found;
    _sender.setType(babel::Message::MessageType::Connect);
    _sender.setBody(name.c_str(), name.size());
    _socket->write(_sender);
    found = waitForCommand(babel::Message::Connect);
    emptyCommandList();
    return found;
}

bool    babel::DataManager::emptyCommandList() {
    while (_eventList.size() > 0) {
        babel::Message  message = _eventList.front();
        _eventList.pop();

        if (_functionPtrs.find(message.getType()) != _functionPtrs.end()) {
            (this->*_functionPtrs[message.getType()])(message);
        }
    }
    return true;
}

bool    babel::DataManager::waitForCommand(babel::Message::MessageType mType) {
    while (true) {
        if (_socket->haveAvailableData()) {
            _eventList.push(_socket->getAvailableMessage());
            if (_eventList.back().getType() == mType) {
                return true;
            }
            else if (_eventList.back().getType() == Message::MessageType::Error) {
                return false;
            }
        }
    }
}

std::string const&  babel::DataManager::getUsername() const {
    return _username;
}

std::string const&  babel::DataManager::getLastError() const {
    return _lastError;
}

void    babel::DataManager::handleConnect(babel::Message const& message) {
    if (message.getBodySize() > 0) {
        _username = std::string(message.getBody(), message.getBodySize());
    }
}

babel::DataManager::~DataManager() {}