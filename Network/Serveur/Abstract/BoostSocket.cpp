//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include <boost/bind.hpp>
#include "BoostSocket.h"

babel::BoostSocket::BoostSocket(boost::asio::io_service& io_service) : Logger("BoostSocket"), _socket(io_service), _readM(babel::Message::MessageType::Unknown) {

}

bool    babel::BoostSocket::connectSocket() {
    return _socket.is_open();
}

boost::asio::ip::tcp::socket&   babel::BoostSocket::getSocket() {
    return _socket;
}

void    babel::BoostSocket::startSession() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readM.data(), Message::headerSize),
                            boost::bind(&babel::BoostSocket::handleReadHeader, this, boost::asio::placeholders::error));
}

void    babel::BoostSocket::handleReadHeader(const boost::system::error_code &error) {
    if (!error) {
        if (_readM.decodeHeader()) {
            boost::asio::async_read(_socket,
                                    boost::asio::buffer((_readM.getBody()), _readM.getBodySize()),
                                    boost::bind(&babel::BoostSocket::handleReadBody, this, boost::asio::placeholders::error));
        }
        else {
            Logger::say("Can't validate header");
        }
    }
    else {
        Logger::say(error.message());
    }
}

void    babel::BoostSocket::handleReadBody(const boost::system::error_code &error) {
    if (!error) {
        std::cout << _readM.getBody() << std::endl;
        addMessage(_readM);
        startSession();
    }
    else {
        Logger::say(error.message());
    }
}


babel::BoostSocket::~BoostSocket() {}