//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

# include <string>
# include <boost/bind.hpp>
# include <boost/asio.hpp>
# include "../Logger/Logger.h"

namespace   babel {
    class Server : private Logger {
    public:
        Server(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint);
        ~Server();

    public:
        void start_accept();

    private:

    private:
        boost::asio::io_service& _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
    };
}

#endif //NETWORK_SERVER_H
