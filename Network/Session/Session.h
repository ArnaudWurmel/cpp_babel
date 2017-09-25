//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

# include <boost/asio.hpp>
# include "Message/Message.h"
# include "../Logger/Logger.h"

namespace babel {
    class Session : private Logger {
    public:
        Session(boost::asio::io_service&);
        ~Session();

    public:
        void    startSession();
        boost::asio::ip::tcp::socket&   getSocket();

    public:
        void    handleReadHeader(const boost::system::error_code& error);

    private:
        boost::asio::ip::tcp::socket    _socket;
        Message _readM;
    };
}

#endif //NETWORK_SESSION_H