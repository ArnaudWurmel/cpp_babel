//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include "BoostAcceptor.h"

babel::BoostAcceptor::BoostAcceptor(unsigned int port, std::mutex& haveAction, std::condition_variable& cv) : Logger("BoostAcceptor"), _endpoint(boost::asio::ip::tcp::v4(), port), _acceptor(_ioService), _haveAction(haveAction), _cv(cv) {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(_endpoint);
    _acceptor.listen();
  say("All initialized");

    startAccept();
}

void    babel::BoostAcceptor::startAccept() {
    std::shared_ptr<BoostSocket>    new_session(new BoostSocket(_ioService, _haveAction, _cv));

    _acceptor.async_accept(new_session->getSocket(),
                           boost::bind(&babel::BoostAcceptor::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void    babel::BoostAcceptor::run() {
    _ioService.run();
}

bool    babel::BoostAcceptor::haveAWaitingClient() {
    _queueLocker.lock();
    bool state = _clientQueue.size() > 1 || (_clientQueue.size() > 0 && _clientQueue.front()->isOpen());
    _queueLocker.unlock();
    return state;
}

std::shared_ptr<babel::ISocket> babel::BoostAcceptor::acceptClient() {
    _queueLocker.lock();
    std::shared_ptr<babel::ISocket> newClient(_clientQueue.front());
    _clientQueue.pop();
    _queueLocker.unlock();
    return newClient;
}

void    babel::BoostAcceptor::stop() {
    _ioService.stop();
}

void    babel::BoostAcceptor::handle_accept(std::shared_ptr<BoostSocket> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->startSession();
        _queueLocker.lock();
        _clientQueue.push(new_session);
        say("Client Connected");
        _queueLocker.unlock();
        _cv.notify_one();
    }
    else {
        say(error.message());
    }
    startAccept();
}

babel::BoostAcceptor::~BoostAcceptor() {}
