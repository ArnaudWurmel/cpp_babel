//
// Created by wurmel_a on 01/10/17.
//

#include <QHostAddress>
#include <iostream>
#include "QtSocket.h"

babel::QtSocket::QtSocket(std::mutex& locker, std::condition_variable& cv, QObject *parent) : QObject(parent), ISocket(locker, cv) {
    _socket = std::unique_ptr<QTcpSocket>(new QTcpSocket());
    _readBody = false;
    connect(_socket.get(), SIGNAL(connected()), this, SLOT(socketConnected()));
}

bool    babel::QtSocket::connectSocket(std::string const& host, unsigned short port) {
    _socket->connectToHost(QString(host.c_str()), port);
    return _socket->waitForConnected(1000);
}

bool    babel::QtSocket::isOpen() const {
    return _socket->state() == QAbstractSocket::ConnectedState;
}

void    babel::QtSocket::startSession() {
    connect(_socket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(_socket.get(), SIGNAL(disconnected()), this, SLOT(connectionClosed()));
}

std::string  babel::QtSocket::getIpAddr() const {
    return _socket->peerAddress().toString().toStdString();
}

void    babel::QtSocket::write(babel::Message message) {
    message.encodeHeader();
    message.encodeData();
    _socket->write(message.data(), message.totalSize());
    _socket->waitForBytesWritten(1000);
}

void    babel::QtSocket::close() {
    _socket->close();
}

void    babel::QtSocket::socketConnected() {
    std::cout << "Socket connected" << std::endl;
}

void    babel::QtSocket::connectionClosed() {
    std::cout << "Socket disconnected" << std::endl;
}

void    babel::QtSocket::readyRead() {
    if (!_readBody) {
        _socket->read(_readMess.data(), babel::Message::headerSize);
        if (_readMess.decodeHeader()) {
            if (_readMess.getBodySize() > 0) {
                _readBody = true;
                readyRead();
            }
            else
                addMessage(_readMess);
        }
    }
    else {
        _socket->read(_readMess.getBody(), _readMess.getBodySize());
        addMessage(_readMess);
    }
}

babel::QtSocket::~QtSocket() {
    if (_socket->isOpen())
        _socket->close();
}