//
// Created by wurmel_a on 01/10/17.
//

#include "DataManager.h"
#include "../Socket/QtSocket.h"
#include "../Exceptions/Exception.h"

babel::DataManager  *babel::DataManager::getInstance(DataManager *inst) {
    static DataManager  *_keeped = NULL;

    if (_keeped == NULL)
        _keeped = inst;
    return _keeped;
}

babel::DataManager::DataManager(Window& win, std::string const& host, unsigned short port) : Logger("DataManager"), _window(win) {
    getInstance(this);
    _continue = true;
    _functionPtrs.insert(std::make_pair(Message::MessageType::Connect, &babel::DataManager::handleConnect));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Error, &babel::DataManager::handleError));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Userlist, &babel::DataManager::handleUserList));
    say("Using : " + host + ":" + std::to_string(port));
    _socket = std::unique_ptr<ISocket>(new babel::QtSocket(_socketReading, _socketWaiter));
    if (!_socket->connectSocket(host, port)) {
        throw NetworkException();
    }
    _synchroniser.lock();
    _socket->startSession();
    _haveInput = false;
}

void    babel::DataManager::executeAction(babel::Message::MessageType type, std::string const &body) {
    _synchroniser.lock();
    _sender.setType(type);
    _sender.setBody(body.c_str(), body.size());
    _haveInput = true;
    _inputWaiter.notify_one();
}

void    babel::DataManager::startData() {
    _dataLooper = std::unique_ptr<std::thread>(new std::thread(&babel::DataManager::senderLoop, this));
}

void    babel::DataManager::senderLoop() {
    while (_continue) {
        std::unique_lock<std::mutex> lck(_lockInput);
        std::unique_lock<std::mutex>    socketData(_socketReading);

        _synchroniser.unlock();
        if (_haveInput || _inputWaiter.wait_for(lck, std::chrono::milliseconds(200)) == std::cv_status::no_timeout) {
            if (!_continue)
                return ;
            _haveInput = false;
            _sender.encodeHeader();
            _sender.encodeData();
            _socket->write(_sender);
            _socketWaiter.wait(socketData);
            if (_continue && _socket->haveAvailableData()) {
                bool    found = false;
                Message::MessageType mType = _sender.getType();

                while (!found && _continue) {
                    if (_socket->haveAvailableData()) {
                        babel::Message  respond = _socket->getAvailableMessage();

                        if (respond.getType() == mType || mType == Message::MessageType::Error) {
                            found = true;
                            if (_functionPtrs.find(respond.getType()) != _functionPtrs.end()) {
                                (this->*_functionPtrs[mType])(respond);
                            }
                        }
                        else if (respond.getType() == babel::Message::Event) {
                            _eventList.push(respond);
                        }
                    }
                    else
                        _socketWaiter.wait(socketData);
                }
            }
        }
    }
}

bool    babel::DataManager::connectUser(std::string const& name) {
    _synchroniser.lock();
    _sender.setType(babel::Message::MessageType::Connect);
    _sender.setBody(name.c_str(), name.size());
    _haveInput = true;
    _inputWaiter.notify_one();
    return true;
}

bool    babel::DataManager::updateUserList() {
    _synchroniser.lock();
    _sender.setType(babel::Message::MessageType::Userlist);
    _sender.setBody("", 0);
    _haveInput = true;
    _inputWaiter.notify_one();
    return true;
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
        say("Username : " + _username);
        emit _window.userConnected(_username);
    }
}

void    babel::DataManager::handleError(babel::Message const& message) {
    if (message.getBodySize() > 0) {
        _lastError = std::string(message.getBody(), message.getBodySize());
    }
}

void    babel::DataManager::handleUserList(babel::Message const& message) {
    std::string body(message.getBody(), message.getBodySize());


}

void    babel::DataManager::clearEventList() {
    while (_eventList.size() > 0) {
        babel::Message  message = _eventList.front();
        _eventList.pop();

        if (message.getType() == babel::Message::MessageType::Event) {
            std::string body(message.getBody(), message.getBodySize());

            if (body.find(" ") != std::string::npos) {
                body = body.substr(0, body.find(" "));
            }
        }
    }
}

void    babel::DataManager::handleCon(std::string const& data) {

}

std::vector<std::string>    babel::DataManager::getTokenFrom(std::string const& body, std::string const &sep) {

}

babel::DataManager::~DataManager() {
    std::cout << "Deleting" << std::endl;
    _continue = false;
    _inputWaiter.notify_one();
    _socketWaiter.notify_one();
    _dataLooper->join();
}