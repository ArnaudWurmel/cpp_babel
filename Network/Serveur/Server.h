//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

# include <set>
# include <string>
# include <boost/bind.hpp>
# include <boost/asio.hpp>
# include "../Logger/Logger.h"
# include "../Session/Session.h"
# include "Abstract/BoostAcceptor.h"
# include "Abstract/BoostSocket.h"
# include "Abstract/ISocketAcceptor.h"

namespace   babel {
    class Server : private Logger {
    public:
        explicit Server(unsigned int port);
        ~Server();

    public:
        void start_accept();

    private:
        std::unique_ptr<babel::ISocketAcceptor> _socketAcceptor;
    };
}

#endif //NETWORK_SERVER_H