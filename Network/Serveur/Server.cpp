//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Server.h"

babel::Server::Server(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint) : Logger("Server"), _io_service(io_service), _acceptor(_io_service, endpoint) {
    Logger::say("Instancied");
}

void    babel::Server::start_accept() {
    std::shared_ptr<Session>    new_session(new Session(_io_service));

    _acceptor.async_accept(new_session->getSocket(),
                            boost::bind(&))
}

void    babel::Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->startSession();
    }
    start_accept();
}

babel::Server::~Server() {}