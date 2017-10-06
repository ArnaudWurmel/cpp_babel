//
// Created by wurmel_a on 06/10/17.
//

#include "SoundManager.h"
#include "../Socket/QtUdpServer.h"

SoundManager::User::User(std::string const &name, std::string const& ip) {
    _name = name;
    _ip = ip;
}

SoundManager::User::~User() {}

SoundManager::SoundManager() : Logger("SoundManager") {
    say("Initialized");
    _running = false;
}

bool    SoundManager::openServer(std::string const& channelName) {
    _channelName = channelName;
    _server = std::unique_ptr<babel::IServer>(new babel::QtUdpServer());
    _server->bind("127.0.0.1", 8888);
    _server->runThread();
    _running = true;
    say("Server runned");
    return true;
}

void    SoundManager::closeServer() {
    _server->stop();
    _server.reset();
    _userList.clear();
    _running = false;
}

bool SoundManager::isRunning() const {
    return _running;
}

void    SoundManager::addUser(std::string const& uInfo) {
    std::cout << uInfo << std::endl;
}

void    SoundManager::stopPlayingAUser(std::string const& uName) {

}

SoundManager::~SoundManager() {

}