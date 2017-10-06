//
// Created by wurmel_a on 06/10/17.
//

#include <DataManager/DataManager.h>
#include <cstring>
#include "SoundManager.h"
#include "../Audio/IAudioCodec.hpp"
#include "../Audio/IAudio.hpp"
#include "../Audio/Play.hpp"
#include "../Audio/Record.hpp"
#include "../Audio/AudioPA.hpp"
#include "../Audio/AudioCodec.hpp"
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
    _running = true;
    _thread = std::unique_ptr<std::thread>(new std::thread(&SoundManager::sendLoop, this));
    say("Server runned");
    return true;
}

void    SoundManager::closeServer() {
    _running = false;
    _server->stop();
    _server.reset();
    _userList.clear();
    _thread->join();
    _thread.reset();
    say("Server stopped");
}

bool SoundManager::isRunning() const {
    return _running;
}

void    SoundManager::addUser(std::string const& uInfo) {
    std::vector<std::string>    userInfoVec = babel::DataManager::getTokenFrom(uInfo, " ");

    if (userInfoVec.size() == 2) {
        _userList.push_back(std::unique_ptr<SoundManager::User>(new SoundManager::User(userInfoVec[0], userInfoVec[1])));
    }
}

void    SoundManager::stopPlayingAUser(std::string const& uName) {
    std::vector<std::unique_ptr<SoundManager::User> >::const_iterator   it = _userList.begin();

    while (it != _userList.end()) {
        if ((*it)->_name.compare(uName) == 0) {
            _userList.erase(it);
            return ;
        }
        ++it;
    }
}

void    SoundManager::sendLoop() {
    std::unique_ptr<IAudioCodec>    codec(new OpusCodec());
    std::unique_ptr<Play> play(new Play());
    std::unique_ptr<Record> rec(new Record());

    play->startAudio();
    rec->startAudio();

    while (_running) {
        if (!play->isActive()) {
            play->stopAudio();
            play->startAudio();
        }
        if (!rec->isActive()) {
            rec->stopAudio();
            rec->startAudio();
        }
        std::vector<std::unique_ptr<SoundManager::User> >::const_iterator   it = _userList.begin();
        EncodedFrame toSend = codec->AudioEncode(rec->RecordedFrames());
        if (toSend.size > 0) {
            babel::Message  message;

            message.setType(babel::Message::MessageType::Audio);
            message.setBody(toSend.frame.data(), toSend.size);
            while (it != _userList.end()) {
                _server->sendFrameTo((*it)->_ip, message);
                ++it;
            }
        }
        if (isRunning() && _server->haveAvailableData()) {
            babel::Message  frameMessage = _server->getAvailableData();

            if (frameMessage.getBodySize() > 0) {
                EncodedFrame    frame;
                frame.frame = std::vector<unsigned char>(frameMessage.getBody(), frameMessage.getBody() + frameMessage.getBodySize());
                frame.size = frameMessage.getBodySize();
                DecodedFrame toPlay = codec->AudioDecode(frame);
            }
        }
    }
}

SoundManager::~SoundManager() {
    if (_running)
        closeServer();
}