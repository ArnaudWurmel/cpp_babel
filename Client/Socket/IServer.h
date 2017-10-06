//
// Created by wurmel_a on 06/10/17.
//

#ifndef CLIENT_ISERVER_H
#define CLIENT_ISERVER_H

# include <iostream>
# include "Message.h"

namespace babel {
    class   IServer {
    public:
        virtual ~IServer() {}

        virtual void    bind(std::string const& addr, unsigned short) = 0;
        virtual void    stop() = 0;
        virtual void    sendFrameTo(std::string const&, babel::Message&) = 0;
        virtual bool    haveAvailableData()  = 0;
        virtual babel::Message  getAvailableData() = 0;
    };
}

#endif //CLIENT_ISERVER_H
