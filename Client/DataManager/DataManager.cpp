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
    _functionPtrs.insert(std::make_pair(Message::MessageType::ChannelList, &babel::DataManager::handleChannelList));
    _functionPtrs.insert(std::make_pair(Message::MessageType::Join, &babel::DataManager::handleJoin));

    _eventPtrs.insert(std::make_pair("con", &babel::DataManager::handleCon));
    _eventPtrs.insert(std::make_pair("disc", &babel::DataManager::handleDisc));
    _eventPtrs.insert(std::make_pair("Join", &babel::DataManager::handleUserJoin));
    _eventPtrs.insert(std::make_pair("Leave", &babel::DataManager::handleUserLeave));
    _eventPtrs.insert(std::make_pair("JoinMe", &babel::DataManager::handleJoinMe));
    say("Using : " + host + ":" + std::to_string(port));
    _socket = std::unique_ptr<ISocket>(new babel::QtSocket(_socketReading, _socketWaiter));
    if (!_socket->connectSocket(host, port)) {
        throw NetworkException();
    }
    _socket->startSession();
    _haveInput = false;
}

void    babel::DataManager::executeAction(babel::Message::MessageType type, std::string const &body) {
    _synchroniser.lock();
    _sender.setType(type);
    _sender.setBody(body.c_str(), body.size());
    _haveInput = true;
    _inputWaiter.notify_one();
    _synchroniser.unlock();
}

void    babel::DataManager::startData() {
    _dataLooper = std::unique_ptr<std::thread>(new std::thread(&babel::DataManager::senderLoop, this));
}

void    babel::DataManager::senderLoop() {
    while (_continue) {
        std::unique_lock<std::mutex> lck(_lockInput);
        std::unique_lock<std::mutex>    socketData(_socketReading);

        if (_haveInput || _inputWaiter.wait_for(lck, std::chrono::milliseconds(200)) == std::cv_status::no_timeout) {
            if (!_continue)
                return ;
            _synchroniser.lock();
            say("Action acquired");
            _haveInput = false;
            if (_sender.getType() != Message::MessageType::Unknown) {
                _sender.encodeHeader();
                _sender.encodeData();
                _socket->write(_sender);
                _socketWaiter.wait(socketData);
            }
            if (_continue && _socket->haveAvailableData()) {
                bool    found = false;
                Message::MessageType mType = _sender.getType();

                _sender.setType(Message::MessageType::Unknown);
                while (!found && _continue) {
                    if (_socket->haveAvailableData()) {
                        babel::Message  respond = _socket->getAvailableMessage();

                        if (mType == Message::MessageType::Unknown && respond.getType() == babel::Message::MessageType::Event) {
                            _eventList.push(respond);
                            found = !_socket->haveAvailableData();
                        }
                        else if (mType != babel::Message::Unknown && (respond.getType() == mType || respond.getType() == Message::MessageType::Error)) {
                            found = true;
                            if (_functionPtrs.find(respond.getType()) != _functionPtrs.end()) {
                                (this->*_functionPtrs[mType])(respond);
                            }
                        }
                        else if (respond.getType() == babel::Message::Event) {
                            _eventList.push(respond);
                        }
                    }
                    else {
                        _socketWaiter.wait(socketData);
                    }
                }
                if (mType == babel::Message::Unknown && _eventList.size() > 0)
                    emit _window.newEvent();
            }
			_synchroniser.unlock();
        }
        if (_socket->haveAvailableData()) {
            while (_socket->haveAvailableData()) {
                babel::Message  message = _socket->getAvailableMessage();

                if (message.getType() == babel::Message::MessageType::Event)
                    _eventList.push(message);
            }
            clearEventList();
        }
    }
}

std::string const&  babel::DataManager::getUsername() const {
    return _username;
}

std::string const&  babel::DataManager::getLastError() const {
    return _lastError;
}

std::string const&  babel::DataManager::getCurrentChannel() const {
    return _channelName;
}

void    babel::DataManager::setCurrentChannelName(std::string const& channelName) {
    _channelName = channelName;
}

void    babel::DataManager::handleConnect(babel::Message const& message) {
    if (message.getBodySize() > 0) {
        _username = std::string((char *)message.getBody(), message.getBodySize());
        say("Username : " + _username);
        emit _window.userConnected(_username);
    }
}

void    babel::DataManager::handleError(babel::Message const& message) {
    if (message.getBodySize() > 0) {
        _lastError = std::string((char *)message.getBody(), message.getBodySize());
    }
    emit _window.errored(std::string((char *)message.getBody(), message.getBodySize()));
}

void    babel::DataManager::handleUserList(babel::Message const& message) {
    std::string body((char *)message.getBody(), message.getBodySize());
    std::vector<std::string>    tokenList = getTokenFrom(body, ";");

    emit _window.updateUList(tokenList);
}

void    babel::DataManager::handleChannelList(babel::Message const& message) {
    std::string body((char *)message.getBody(), message.getBodySize());
    std::vector<std::string>    tokenList = getTokenFrom(body, ";");

    emit _window.updateChanList(tokenList);
}

void    babel::DataManager::handleJoin(babel::Message const& message) {
    std::vector<std::string>    body = getTokenFrom(std::string((char *)message.getBody(), message.getBodySize()), ";");
    say(std::string((char *)message.getBody(), message.getBodySize()));
    emit _window.joinSuccess(body);
}

void    babel::DataManager::clearEventList() {
    while (_eventList.size() > 0) {
        babel::Message  message = _eventList.front();
        _eventList.pop();

        if (message.getType() == babel::Message::MessageType::Event && message.getBodySize() > 0) {
            std::vector<std::string>    tokenList = getTokenFrom(std::string((char *)message.getBody(), message.getBodySize()), " ");

            if (tokenList.size() > 0) {
                if (_eventPtrs.find(tokenList[0]) != _eventPtrs.end()) {
                    (this->*_eventPtrs[tokenList[0]])(tokenList);
                }
            }
        }
    }
}

void    babel::DataManager::handleCon(std::vector<std::string> const& data) {
    if (data.size() > 1) {
        std::string username;
        std::vector<std::string>::const_iterator  it = data.begin() + 1;

        while (it != data.end()) {
            username = username + *it;
            ++it;
            if (it != data.end()) {
                username += " ";
            }
        }
        emit _window.userConnected(username);
    }
}

void    babel::DataManager::handleDisc(std::vector<std::string> const& data) {
    if (data.size() > 1) {
        std::string username;
        std::vector<std::string>::const_iterator  it = data.begin() + 1;

        while (it != data.end()) {
            username = username + *it;
            ++it;
            if (it != data.end()) {
                username += " ";
            }
        }
        emit _window.userDisconnected(username);
    }
}

void    babel::DataManager::handleUserJoin(std::vector<std::string> const& data) {
    if (data.size() > 2) {
        std::string channelName = data.back();
        std::string username = data[1];

        emit _window.joinEvent(username, channelName);
    }
}

void    babel::DataManager::handleUserLeave(std::vector<std::string> const& data) {
    if (data.size() > 2) {
        std::string channelName = data.back();
        std::string username = data[1];

        emit _window.leaveEvent(username, channelName);
    }
}

void    babel::DataManager::handleJoinMe(std::vector<std::string> const& uInfo) {
    if (uInfo.size() == 3) {
        std::string username = uInfo[1];
        std::string ip = uInfo[2];

        emit _window.joinMeEvent(username, ip);
    }
}

std::vector<std::string>    babel::DataManager::getTokenFrom(std::string const& body, std::string const &sep) {
    std::vector<std::string>    tokenList;
    std::string::const_iterator it;

    it = body.begin();
    while (it != body.end()) {
        std::string token;

        while (it != body.end() && sep.find(*it) == std::string::npos) {
            token += *it;
            ++it;
        }
        if (token.size()) {
            tokenList.push_back(token);
            token.clear();
        }
        while (it != body.end() && sep.find(*it) != std::string::npos)
            ++it;
    }
    return tokenList;
}

babel::DataManager::~DataManager() {
    _continue = false;
    _inputWaiter.notify_one();
    _socketWaiter.notify_one();
    _dataLooper->join();
}