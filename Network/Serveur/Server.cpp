//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Server.h"

babel::Server::Server(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint) : Logger("Server"), _io_service(io_service), _acceptor(_io_service, endpoint) {
    Logger::say("Instancied");
}

void    babel::Server::start_accept() {
    _acceptor.async_accept()
}

babel::Server::~Server() {}