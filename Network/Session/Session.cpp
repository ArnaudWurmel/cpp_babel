//
// Created by wurmel_a on 25/09/17.
//

#include "Session.h"

babel::Session::Session(boost::asio::io_service& io_service) : _socket(io_service) {

}

boost::asio::ip::tcp::socket&   babel::Session::getSocket() {
    return _socket;
}

void    babel::Session::startSession() {

}

babel::Session::~Session() {}

