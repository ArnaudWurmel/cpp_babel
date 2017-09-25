//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

# include <boost/asio.hpp>

namespace babel {
    class Session {
    public:
        Session(boost::asio::io_service&);
        ~Session();

    public:
        void    startSession();
        boost::asio::ip::tcp::socket&   getSocket();

    public:


    private:
        boost::asio::ip::tcp::socket    _socket;
    };
}

#endif //NETWORK_SESSION_H