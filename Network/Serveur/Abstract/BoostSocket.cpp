//
// Created by wurmel_a on 27/09/17.
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "BoostSocket.h"

babel::BoostSocket::BoostSocket(boost::asio::io_service& io_service, std::mutex& haveData, std::condition_variable& cv) : ISocket(haveData, cv), Logger("BoostSocket"), _socket(io_service), _readM(babel::Message::MessageType::Unknown) {

}

bool    babel::BoostSocket::isOpen() const {
    return _socket.is_open();
}

bool    babel::BoostSocket::connectSocket(std::string const& host, unsigned short port) {
    boost::asio::ip::tcp::endpoint  endpoint(boost::asio::ip::address::from_string(host), port);
    boost::system::error_code   code;

    _socket.connect(endpoint, code);
    return !code && _socket.is_open();
}

void    babel::BoostSocket::close() {
    if (_socket.is_open()) {
        _socket.cancel();
        _socket.close();
        _cv.notify_one();
    }
}

boost::asio::ip::tcp::socket&   babel::BoostSocket::getSocket() {
    return _socket;
}

void    babel::BoostSocket::startSession() {
    boost::asio::async_read(_socket,
                            boost::asio::buffer(_readM.data(), Message::headerSize),
                            boost::bind(&babel::BoostSocket::handleReadHeader, shared_from_this(), boost::asio::placeholders::error));
}

void    babel::BoostSocket::handleReadHeader(const boost::system::error_code &error) {
    if (!error) {
        if (_readM.decodeHeader()) {
            boost::asio::async_read(_socket,
                                    boost::asio::buffer((_readM.getBody()), _readM.getBodySize()),
                                    boost::bind(&babel::BoostSocket::handleReadBody, shared_from_this(), boost::asio::placeholders::error));
        }
        else {
            Logger::say("Can't validate header");
            close();
        }
    }
    else {
        Logger::say(error.message());
        close();
    }
}

void    babel::BoostSocket::handleReadBody(const boost::system::error_code &error) {
    if (!error) {
        addMessage(_readM);
        startSession();
    }
    else {
        Logger::say(error.message());
        close();
    }
}

std::string babel::BoostSocket::getIpAddr() const {
    return _socket.remote_endpoint().address().to_string();
}

void    babel::BoostSocket::write(babel::Message message) {
    _mutex.lock();
    bool    onWriting = !_writeList.empty();
    _writeList.push(message);
    _writeList.back().encodeHeader();
    _writeList.back().encodeData();
    _mutex.unlock();
    if (!onWriting) {
        boost::asio::async_write(_socket,
                                 boost::asio::buffer(_writeList.front().data(),
                                                     _writeList.front().totalSize()),
                                 boost::bind(&babel::BoostSocket::handleWrite, shared_from_this(),
                                             boost::asio::placeholders::error));
    }
}

void    babel::BoostSocket::handleWrite(const boost::system::error_code &error) {
    if (!error) {
        std::cout << "Writed : " << std::string(_writeList.front().getBody(), _writeList.front().getBodySize());
        say("Sended");
        _mutex.lock();
        _writeList.pop();
        if (!_writeList.empty()) {
            _mutex.unlock();
            boost::asio::async_write(_socket,
                                     boost::asio::buffer(_writeList.front().data(),
                                                         _writeList.front().totalSize()),
                                     boost::bind(&babel::BoostSocket::handleWrite, shared_from_this(),
                                                 boost::asio::placeholders::error));
            return ;
        }
        _mutex.unlock();
    }
    else {
        say(error.message());
        close();
    }
}


babel::BoostSocket::~BoostSocket() {}