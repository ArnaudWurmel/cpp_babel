//
// Created by wurmel_a on 06/10/17.
//

#ifndef CLIENT_SOUNDMANAGER_H
#define CLIENT_SOUNDMANAGER_H

# include <iostream>
#include <bits/unique_ptr.h>
#include <vector>
# include "../Socket/IServer.h"
# include "../Logger/Logger.h"

class SoundManager : public babel::Logger {
public:
    struct User {
        User(std::string const&, std::string const&);
        ~User();
        std::string _name;
        std::string _ip;
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
    bool    _running;
    std::unique_ptr<babel::IServer> _server;
    std::string _channelName;
    std::vector<std::unique_ptr<SoundManager::User> >   _userList;
};


#endif //CLIENT_SOUNDMANAGER_H
