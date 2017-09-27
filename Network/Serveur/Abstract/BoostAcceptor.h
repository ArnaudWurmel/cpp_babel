//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_BOOSTACCEPTOR_H
#define NETWORK_BOOSTACCEPTOR_H

# include <boost/bind.hpp>
# include <mutex>
# include <boost/asio.hpp>
# include <queue>
#include <condition_variable>
# include "ISocketAcceptor.h"
# include "BoostSocket.h"

namespace babel {
    class BoostAcceptor : public ISocketAcceptor, public Logger {
    public:
        BoostAcceptor(unsigned int, std::mutex&, std::condition_variable&);
        ~BoostAcceptor();

    public:
        void    run();
        void    startAccept();
        bool    haveAWaitingClient();
        std::shared_ptr<babel::ISocket> acceptClient();

    private:
        void handle_accept(std::shared_ptr<BoostSocket>, const boost::system::error_code& error);

    private:
        boost::asio::ip::tcp::endpoint  _endpoint;
        boost::asio::io_service    _ioService;
        boost::asio::ip::tcp::acceptor _acceptor;
        std::queue<std::shared_ptr<babel::BoostSocket> >    _clientQueue;
        std::mutex  _queueLocker;

    private:
        std::mutex& _haveAction;
        std::condition_variable& _cv;
    };
}

#endif //NETWORK_BOOSTACCEPTOR_H
