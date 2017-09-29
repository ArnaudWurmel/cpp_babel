//
// Created by wurmel_a on 26/09/17.
//

#ifndef NETWORK_USER_H
#define NETWORK_USER_H

# include <string>
# include <vector>
# include "../Logger/Logger.h"
# include "../Serveur/Abstract/ISocket.h"

namespace babel {
    class User {
    public:
        User(std::shared_ptr<babel::ISocket>);
        ~User();

    public:
        void    initNetwork();
        void    setUsername(std::string const&);
        std::string const&  getUsername() const;
        void    setIpAddr(std::string const&);
        std::string const&  getIpAddr() const;
        bool    manageData();

    private:
        std::string _username;
        std::string _ipAddr;
        std::shared_ptr<babel::ISocket> _socket;
        bool    _continue;
    };
}

#endif //NETWORK_USER_H
