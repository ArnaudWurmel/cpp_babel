//
// Created by wurmel_a on 27/09/17.
//

#include "BoostAcceptor.h"

babel::BoostAcceptor::BoostAcceptor(unsigned int port) : Logger("BoostAcceptor"), _endpoint(boost::asio::ip::tcp::v4(), port), _acceptor(_ioService, _endpoint) {
    say("All initialized");
    startAccept();
}

void    babel::BoostAcceptor::startAccept() {
    std::shared_ptr<BoostSocket>    new_session(new BoostSocket(_ioService));
    _queueLocker.lock();
    _clientQueue.push(new_session);
    _queueLocker.unlock();
    _acceptor.async_accept(new_session->getSocket(),
                           boost::bind(&babel::BoostAcceptor::handle_accept, this, new_session, boost::asio::placeholders::error));

}

void    babel::BoostAcceptor::run() {
    _ioService.run();
}

bool    babel::BoostAcceptor::haveAWaitingClient() {
    _queueLocker.lock();
    bool state = _clientQueue.size() > 1 || _clientQueue.front()->connectSocket();
    _queueLocker.unlock();
    return state;
}

std::shared_ptr<babel::ISocket> babel::BoostAcceptor::acceptClient() {
    _queueLocker.lock();
    std::shared_ptr<babel::ISocket> newClient(_clientQueue.front());

    _clientQueue.pop();
    return newClient;
}

void    babel::BoostAcceptor::handle_accept(std::shared_ptr<BoostSocket> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->startSession();
    }
    else {
        say(error.message());
    }
    startAccept();
}

babel::BoostAcceptor::~BoostAcceptor() {}