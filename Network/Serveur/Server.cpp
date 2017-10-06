//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include <functional>
#include "Server.h"

babel::Server::Server(unsigned int port) : Logger("Server") {
    Logger::say("Instancied");
    _channelList.insert(std::unique_ptr<Channel>(new Channel("General")));
    _channelList.insert(std::unique_ptr<Channel>(new Channel("General2")));

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
        std::set<std::shared_ptr<User> >::iterator  it = _userList.begin();

        while (it != _userList.end()) {
            if (!(*it)->manageData()) {
                std::string body = "disc " + (*it)->getUsername();
                leaveChannel(*(*it).get());
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
        pseudo = "Invité#" + std::to_string(user.getId());
    }
    else {
        pseudo = std::string(message.getBody(), message.getBodySize());
    }
    std::cout << pseudo << std::endl;
   std::set<std::shared_ptr<User> >::const_iterator    it = _userList.begin();
    bool taken = false;

    while (!taken && it != _userList.end()) {
        if ((*it)->getUsername().compare(pseudo) == 0) {
            taken = true;
        }
        ++it;
    }
    std::cout << taken << " " << pseudo.size() << " " << validateName(pseudo) << std::endl;
    if (taken || pseudo.size() >= babel::Message::maxBodySize || !validateName(pseudo)) {
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
    std::set<std::shared_ptr<babel::User> >::const_iterator it = _userList.begin();
    std::string res = "";

    while (it != _userList.end()) {
        if ((*it)->getUsername().size()) {
            res = res + (*it)->getUsername() + ";";
        }
        ++it;
    }
    std::cout << "Res : <" << res << ">" << std::endl;
    caller.sendResponse(babel::Message::MessageType::Userlist, res);
}

void    babel::Server::channelList(babel::User& caller, babel::Message& message) {
    std::set<std::unique_ptr<Channel> >::const_iterator it = _channelList.begin();
    std::string res;

    while (it != _channelList.end()) {
        res = res + (*it)->getName();
        std::set<std::shared_ptr<User> >::const_iterator    itUser = (*it)->getUserList().begin();

        while (itUser != (*it)->getUserList().end()) {
            res = res + "," + (*itUser)->getUsername();
            ++itUser;
        }
        res += ";";
        ++it;
    }
    say("Res : " + res);
    caller.sendResponse(babel::Message::MessageType::ChannelList, res);
}

void    babel::Server::joinChannel(babel::User& caller, babel::Message& message) {
    if (message.getBodySize() == 0) {
        caller.sendResponse(babel::Message::MessageType::Error, "Missing channel arg");
    }
    else {
        std::set<std::shared_ptr<User> >::const_iterator    itUser = _userList.begin();

        while (itUser != _userList.end() && (*itUser)->getId() != caller.getId()) {
            ++itUser;
        }
        if (itUser != _userList.end()) {
            caller.leaveChannel();
            std::string channelName(message.getBody(), message.getBodySize());
            std::set<std::unique_ptr<Channel> >::iterator   it = _channelList.begin();

            while (it != _channelList.end()) {
                if ((*it)->getName().compare(channelName) == 0) {
                    std::set<std::shared_ptr<User> >::const_iterator    itChannelMember = (*it)->getUserList().begin();
                    std::string res;

                    while (itChannelMember != (*it)->getUserList().end()) {
                        res = res + (*itChannelMember)->getUsername() + " " + (*itChannelMember)->getIpAddr() + ";";
                        ++itChannelMember;
                    }
                    caller.sendResponse(babel::Message::MessageType::Join, res);
                    (*it)->join(*itUser);
                    std::string triggerBody = "Join " + caller.getUsername() + " " + (*it)->getName();
                    triggerEvent(caller.getId(), triggerBody);
                    triggerBody = "JoinMe " + caller.getUsername() + " " + caller.getIpAddr();
                    (*it)->triggerChannelEvent(caller, triggerBody);
                    return ;
                }
                ++it;
            }
            caller.sendResponse(babel::Message::MessageType::Error, "Unknown channelName");
            return ;
        }
        caller.sendResponse(babel::Message::MessageType::Error, "Unknown user");
    }
}

void    babel::Server::leaveChannelFor(babel::User& caller, babel::Message& message) {
    caller.leaveChannel();
    caller.sendResponse(Message::MessageType::Leave, "OK");
}

void    babel::Server::createChannel(babel::User& caller, babel::Message& message) {
    if (message.getBodySize() > 0) {
        std::string channelName(message.getBody(), message.getBodySize());

        if (validateName(channelName)) {
            std::set<std::unique_ptr<Channel> >::const_iterator it = _channelList.begin();

            while (it != _channelList.end()) {
                if ((*it)->getName().compare(channelName) == 0) {
                    caller.sendResponse(Message::MessageType::Error, "Channel name already used");
                    return ;
                }
                ++it;
            }
            _channelList.insert(std::unique_ptr<Channel>(new Channel(channelName)));
            caller.sendResponse(Message::MessageType::Error, "OK");
            return ;
        }
        caller.sendResponse(Message::MessageType::Error, "Can't validate channelName");
        return ;
    }
    caller.sendResponse(Message::MessageType::Error, "Missing channelName arg");
}

void    babel::Server::triggerEvent(unsigned int senderId, std::string const& body) {
    std::set<std::shared_ptr<User> >::iterator  it = _userList.begin();

    while (it != _userList.end()) {
        if ((*it)->getId() != senderId) {
            (*it)->sendResponse(babel::Message::MessageType::Event, body);
        }
        ++it;
    }
}

void    babel::Server::leaveChannel(babel::User& user) {
    std::set<std::shared_ptr<babel::User> >::const_iterator itUser = _userList.begin();

    while (itUser != _userList.end() && (*itUser)->getId() != user.getId()) {
        ++itUser;
    }
    if (itUser == _userList.end())
        return ;
    std::set<std::unique_ptr<babel::Channel> >::iterator    it = _channelList.begin();

    while (it != _channelList.end()) {
        if ((*it)->leave(*itUser)) {
            triggerEvent(user.getId(), "Leave " + user.getUsername() + " " + (*it)->getName());
            return ;
        }
        ++it;
    }
}

bool    babel::Server::validateName(std::string const& name) {
    std::string::const_iterator it = name.begin();

    while (it != name.end()) {
        char c = (*it);
        if (!((c >= 'A' || c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')) {
            return false;
        }
        ++it;
    }
    return true;
}

babel::Server::~Server() {
    _cv.notify_one();
    _acceptorThread->join();
    _acceptorThread.reset();
    _userList.clear();
    _socketAcceptor->stop();
    _socketAcceptor.reset();
}
