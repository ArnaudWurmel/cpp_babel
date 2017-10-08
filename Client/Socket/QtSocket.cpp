//
// Created by wurmel_a on 01/10/17.
//

#include <QHostAddress>
#include <iostream>
#include <thread>
#include <QtCore/QCoreApplication>
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
    connect(this, SIGNAL(flushing()), SLOT(needFlushing()));
}

std::string  babel::QtSocket::getIpAddr() const {
    return _socket->peerAddress().toString().toStdString();
}

void    babel::QtSocket::write(babel::Message message) {
    _writingList.push(message);
    emit flushing();
}

void    babel::QtSocket::close() {
    _socket->close();
}

void    babel::QtSocket::socketConnected() {
    std::cout << "Socket connected" << std::endl;
}

void    babel::QtSocket::connectionClosed() {
    std::cout << "Socket disconnected" << std::endl;
    QCoreApplication::quit();
}

void    babel::QtSocket::readyRead() {
    if (!_readBody) {
        _socket->read(reinterpret_cast<char *>(_readMess.data()), babel::Message::headerSize);
        if (_readMess.decodeHeader()) {
            if (_readMess.getBodySize() > 0) {
                _readBody = true;
                if (_socket->isReadable())
                    readyRead();
            }
            else {
                addMessage(_readMess);
                if (_socket->bytesAvailable() >= babel::Message::headerSize)
                    readyRead();
            }
        }
    }
    else {
        _socket->read(reinterpret_cast<char *>(_readMess.getBody()), _readMess.getBodySize());
        std::cout << "Body readed : " << std::string(reinterpret_cast<char *>(_readMess.getBody()), _readMess.getBodySize()) << std::endl;
        addMessage(_readMess);
        _readBody = false;
        if (_socket->bytesAvailable() >= babel::Message::headerSize)
            readyRead();
    }
}

void    babel::QtSocket::needFlushing() {
    babel::Message  message = _writingList.front();
    
    message.encodeHeader();
    message.encodeData();
    _socket->write(reinterpret_cast<char *>(message.data()), message.totalSize());
    _socket->waitForBytesWritten(1000);
    _writingList.pop();
}

babel::QtSocket::~QtSocket() {
    if (_socket->isOpen())
        _socket->close();
}