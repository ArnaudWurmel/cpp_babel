//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

# include <string>
# include <boost/bind.hpp>
# include <boost/asio.hpp>
# include "../Logger/Logger.h"
# include "../Session/Session.h"

namespace   babel {
    class Server : private Logger {
    public:
        Server(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint);
        ~Server();

    public:
        void start_accept();
        void handle_accept(std::shared_ptr<Session>, const boost::system::error_code& error);

    private:

    private:
        boost::asio::io_service& _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<Session> >  _session;
    };
}

#endif //NETWORK_SERVER_H
