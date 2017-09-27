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
        User(babel::ISocket&);
        virtual ~User();

    protected:
        void    setUsername(std::string const&);
        std::string const&  getUsername() const;
        void    setIpAddr(std::string const&);
        std::string const&  getIpAddr() const;

    private:
        std::string _username;
        std::string _ipAddr;
        babel::ISocket& _socket;
    };
}

#endif //NETWORK_USER_H
