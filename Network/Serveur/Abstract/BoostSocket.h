//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_BOOSTSOCKET_H
#define NETWORK_BOOSTSOCKET_H

# include <boost/asio.hpp>
# include "ISocket.h"
# include "Message/Message.h"
# include "../../Logger/Logger.h"


namespace babel {
    class BoostSocket : public ISocket, private Logger {
    public:
        BoostSocket(boost::asio::io_service&);
        ~BoostSocket();

    public:
        boost::asio::ip::tcp::socket&   getSocket();
        bool    connectSocket();
        void    startSession();

    private:
        void    handleReadHeader(const boost::system::error_code& error);
        void    handleReadBody(const boost::system::error_code& error);

    private:
        boost::asio::ip::tcp::socket    _socket;
        Message _readM;
    };
}

#endif //NETWORK_BOOSTSOCKET_H
