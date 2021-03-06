//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_BOOSTSOCKET_H
#define NETWORK_BOOSTSOCKET_H

# include <mutex>
# include <condition_variable>
# include <boost/asio.hpp>
# include <memory>
# include "ISocket.h"
# include "Message/Message.h"
# include "../../Logger/Logger.h"


namespace babel {
    class BoostSocket : public ISocket, private Logger, public std::enable_shared_from_this<BoostSocket> {
    public:
        BoostSocket(boost::asio::io_service&, std::mutex&, std::condition_variable&);
        ~BoostSocket();

    public:
        void    close();
        bool    isOpen() const;
        boost::asio::ip::tcp::socket&   getSocket();
        bool    connectSocket(std::string const&, unsigned short);
        void    startSession();
        std::string getIpAddr() const;
        void    write(babel::Message);

    private:
        void    handleReadHeader(const boost::system::error_code& error);
        void    handleReadBody(const boost::system::error_code& error);
        void    handleWrite(boost::system::error_code const&);


    private:
        boost::asio::ip::tcp::socket    _socket;
        Message _readM;
        std::mutex  _mutex;
        std::queue<babel::Message>  _writeList;
    };
}

#endif //NETWORK_BOOSTSOCKET_H
