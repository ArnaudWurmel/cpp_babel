//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "BoostSocket.h"

babel::BoostSocket::BoostSocket(boost::asio::io_service& io_service, std::mutex& haveData, std::condition_variable& cv) : ISocket(haveData, cv), Logger("BoostSocket"), _socket(io_service), _readM(babel::Message::MessageType::Unknown) {

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
            _socket.close();
            notifyMain();
        }
    }
    else {
        Logger::say(error.message());
        _socket.close();
        notifyMain();
    }
}

bool    babel::BoostSocket::isOpen() const {
    return _socket.is_open();
}

void    babel::BoostSocket::handleReadBody(const boost::system::error_code &error) {
    if (!error) {
        addMessage(_readM);
        startSession();
    }
    else {
        Logger::say("Reading body : " + error.message());
        _socket.close();
        notifyMain();
    }
}

std::string babel::BoostSocket::getIpAddr() const {
    return boost::lexical_cast<std::string>(_socket.remote_endpoint());
}

void    babel::BoostSocket::write(babel::Message toDeliver) {
    _queueLocker.lock();
    bool    writeState = _writeList.empty();
    _writeList.push(toDeliver);
    _queueLocker.unlock();
    if (writeState) {
        //boost::asio::async_write(_socket, boost::asio::buffer(_writeList.front().data(), _writeList.front().totalSize()), boost::bind(&babel::BoostSocket::handle_write, this, boost::asio::placeholders::error));
    }
}

void    babel::BoostSocket::handle_write(const boost::system::error_code& error) {
    if (!error) {
        _queueLocker.lock();
        _writeList.pop();
        if (!_writeList.empty()) {
            //boost::asio::async_write(_socket, boost::asio::buffer(_writeList.front().data(), _writeList.front().totalSize()), boost::bind(&babel::BoostSocket::handle_write, this, boost::asio::placeholders::error));
        }
        _queueLocker.unlock();
    }
    else {
        say(error.message());
    }
}


babel::BoostSocket::~BoostSocket() {}