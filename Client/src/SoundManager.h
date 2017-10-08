//
// Created by wurmel_a on 06/10/17.
//

#ifndef CLIENT_SOUNDMANAGER_H
#define CLIENT_SOUNDMANAGER_H

# include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <Audio/Play.hpp>
# include "../Socket/IServer.h"
# include "../Logger/Logger.h"
# include "../Audio/Record.hpp"

class SoundManager : public babel::Logger {
public:
    struct User {
        User(std::string const&, std::string const&, IPlay *);
        ~User();

        std::string _name;
        std::string _ip;
        std::unique_ptr<IPlay>   _play;
    };

public:
    SoundManager();
    ~SoundManager();

public:
    void    closeServer();
    bool    openServer(std::string const&);
    bool    isRunning() const;
    void    addUser(std::string const&);
    void    stopPlayingAUser(std::string const&);

private:
    void    sendLoop(IRecorder *);
    void    getLoop();

private:
    std::unique_ptr<std::thread>    _thread;
    std::unique_ptr<std::thread>    _threadRead;
    bool    _running;
    std::unique_ptr<babel::IServer> _server;
    std::string _channelName;
    std::vector<std::unique_ptr<SoundManager::User> >   _userList;
    std::mutex  _lockGet;
    std::mutex  _lockSend;
};


#endif //CLIENT_SOUNDMANAGER_H
