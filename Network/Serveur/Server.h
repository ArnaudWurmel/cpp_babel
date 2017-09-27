//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

# include <set>
# include <string>
# include <boost/bind.hpp>
# include <boost/asio.hpp>
#include <thread>
#include <condition_variable>
# include "../Logger/Logger.h"
# include "Abstract/BoostAcceptor.h"
# include "Abstract/BoostSocket.h"
# include "Abstract/ISocketAcceptor.h"

namespace   babel {
    class Server : private Logger {
    public:
        explicit Server(unsigned int port);
        ~Server();

    private:
        void    threadLoop();

    private:
        std::unique_ptr<babel::ISocketAcceptor> _socketAcceptor;
        std::unique_ptr<std::thread>    _acceptorThread;
        bool    _threadRunning;
        std::mutex  _haveAction;
        std::condition_variable _cv;
    };
}

#endif //NETWORK_SERVER_H