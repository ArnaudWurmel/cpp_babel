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

SoundManager::User::User(std::string const &name, std::string const& ip, Play *play) {
    _name = name;
    _ip = ip;
    _play = std::unique_ptr<Play>(play);
    _play->startAudio();
}

SoundManager::User::~User() {
    _play->stopAudio();
}

SoundManager::SoundManager() : Logger("SoundManager") {
    say("Initialized");
    _running = false;
}

bool    SoundManager::openServer(std::string const& channelName) {
    _channelName = channelName;
    _server = std::unique_ptr<babel::IServer>(new babel::QtUdpServer());
    _server->bind("0.0.0.0", 8888);
    _running = true;
    Record  *rec = new Record;
    _threadRead = std::unique_ptr<std::thread>(new std::thread(&SoundManager::getLoop, this));
    _thread = std::unique_ptr<std::thread>(new std::thread(&SoundManager::sendLoop, this, rec));
    say("Server runned");
    return true;
}

void    SoundManager::closeServer() {
    _running = false;
    _thread->join();
    _thread.reset();
    _threadRead->join();
    _threadRead.reset();
    _server->stop();
    _server.reset();
    _userList.clear();
    say("Server stopped");
}

bool SoundManager::isRunning() const {
    return _running;
}

void    SoundManager::addUser(std::string const& uInfo) {
    std::vector<std::string>    userInfoVec = babel::DataManager::getTokenFrom(uInfo, " ");

    if (userInfoVec.size() == 2) {
        _userList.push_back(std::unique_ptr<SoundManager::User>(new SoundManager::User(userInfoVec[0], userInfoVec[1], new Play())));
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

void    SoundManager::getLoop() {
    std::unique_ptr<IAudioCodec>    codec(new OpusCodec());

    while (_running) {
        if (isRunning() && _server->haveAvailableData()) {
            std::pair<std::string, babel::Message>  frameMessage = _server->getAvailableData();

            if (frameMessage.second.getBodySize() > 0) {
                EncodedFrame    frame;
                frame.frame = std::vector<unsigned char>(frameMessage.second.getBody(), frameMessage.second.getBody() + frameMessage.second.getBodySize());
                frame.size = frameMessage.second.getBodySize();
                DecodedFrame toPlay = codec->AudioDecode(frame);
                std::vector<std::unique_ptr<User> >::iterator   it = _userList.begin();

                while (it != _userList.end()) {
                    std::cout << (*it)->_name << " " << frameMessage.first << std::endl;
                    if ((*it)->_ip.compare(frameMessage.first) == 0) {
                        std::cout << "Played" << std::endl;
                        (*it)->_play->PlayFrames(toPlay);
                    }
                    ++it;
                }
                Pa_Sleep(5);
            }
        }
    }
}

void    SoundManager::sendLoop(Record *recPtr) {
    std::unique_ptr<IAudioCodec>    codec(new OpusCodec());
    std::unique_ptr<Record> rec(recPtr);

    rec->startAudio();

    while (_running) {
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
    }
}

SoundManager::~SoundManager() {
    if (_running)
        closeServer();
}