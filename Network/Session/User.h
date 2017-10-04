//
// Created by wurmel_a on 26/09/17.
//

#ifndef NETWORK_USER_H
#define NETWORK_USER_H

# include <string>
# include <vector>
# include <functional>
# include <utility>
# include <map>
# include "../Logger/Logger.h"
# include "../Serveur/Abstract/ISocket.h"

namespace babel {
    class Server;
    class User : private Logger {
    public:
        User(std::shared_ptr<babel::ISocket>, babel::Server&);
        ~User();

    public:
        void    initNetwork();
        void    setUsername(std::string const&);
        std::string const&  getUsername() const;
        void    setIpAddr(std::string const&);
        std::string const&  getIpAddr() const;
        bool    manageData();
        unsigned int    getId() const;
        void    sendResponse(babel::Message::MessageType, std::string const&);
        void    leaveChannel();

    private:
        std::string _username;
        std::string _ipAddr;
        unsigned int    _id;
        std::shared_ptr<babel::ISocket> _socket;
        bool    _continue;

    private:
        std::map<babel::Message::MessageType, void (babel::Server::*)(babel::User&, babel::Message&) >   _functionPtrs;
        babel::Server   &_server;

    private:
        static unsigned int _userId;
    };
}

#endif //NETWORK_USER_H
