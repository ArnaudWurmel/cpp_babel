//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Server.h"

babel::Server::Server(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint) : Logger("Server"), _io_service(io_service), _acceptor(_io_service, endpoint) {
    Logger::say("Instancied");
    start_accept();
}

void    babel::Server::start_accept() {
    std::shared_ptr<Session>    new_session(new Session(_io_service));

    _acceptor.async_accept(new_session->getSocket(),
                            boost::bind(&babel::Server::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void    babel::Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code &error) {
    if (!error) {
        std::cout << "Error : " << error << std::endl;
        new_session->startSession();
    }
    start_accept();
}

babel::Server::~Server() {}
