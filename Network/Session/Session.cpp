//
// Created by wurmel_a on 25/09/17.
//

#include <boost/bind.hpp>
#include <iostream>
#include "Session.h"

babel::Session::Session(boost::asio::io_service& io_service) : Logger("Session"), _socket(io_service), _readM(Message::MessageType::Unknown) {

}

boost::asio::ip::tcp::socket&   babel::Session::getSocket() {
    return _socket;
}

void    babel::Session::startSession() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readM.data(), Message::headerSize),
                            boost::bind(&babel::Session::handleReadHeader, this, boost::asio::placeholders::error));
}

void    babel::Session::handleReadHeader(const boost::system::error_code &error) {
    std::cout << error.message() << std::endl;
    if (!error && _readM.validateData()) {
        std::cout << "here" << std::endl;
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_readM.getBody(), _readM.getBodySize()),
                                boost::bind(&babel::Session::handleReadBody, this, boost::asio::placeholders::error));
    }
    else {
        Logger::say("Can't validate header");
    }
}

void    babel::Session::handleReadBody(const boost::system::error_code &error) {
    if (!error) {
        startSession();
    }
}

babel::Session::~Session() {}

