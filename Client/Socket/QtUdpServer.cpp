//
// Created by wurmel_a on 06/10/17.
//

#include "QtUdpServer.h"
#include "Message.h"

babel::QtUdpServer::QtUdpServer() {}

void babel::QtUdpServer::bind(std::string const& host, unsigned short port) {
    _readHeader = true;
    _socket.bind(QHostAddress(host.c_str()), port);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
}

void    babel::QtUdpServer::stop() {
    _socket.close();
}

void    babel::QtUdpServer::readReady() {
    QHostAddress    sender;
    quint16 port;

    if (_readHeader) {
        _socket.readDatagram(_inMess.data(), babel::Message::headerSize, &sender, &port);
        if (_inMess.decodeHeader()) {
            if (_inMess.getBodySize() > 0) {
                _readHeader = false;
                readReady();
            }
        }
    }
    else {
        _socket.readDatagram(_inMess.getBody(), _inMess.getBodySize(), &sender, &port);
        _queueLocker.lock();
        _frameList.push(std::make_pair(sender.toString().toStdString(), _inMess));
        _readHeader = true;
        _queueLocker.unlock();
        std::cout << "Frame readed" << std::endl;
    }
    if (_socket.bytesAvailable() >= babel::Message::headerSize) {
        readReady();
    }
}

void    babel::QtUdpServer::sendFrameTo(std::string const& addr, babel::Message& frame) {
    frame.encodeHeader();
    frame.encodeData();
    _socket.writeDatagram(frame.data(), frame.totalSize(), QHostAddress(addr.c_str()), 8888);
}

bool    babel::QtUdpServer::haveAvailableData() {
    _queueLocker.lock();
    bool state = _frameList.size() > 0;
    _queueLocker.unlock();
    return state;
}

babel::Message  babel::QtUdpServer::getAvailableData() {
    _queueLocker.lock();
    babel::Message  message = _frameList.front().second;
    _frameList.pop();
    _queueLocker.unlock();
    return message;
}

babel::QtUdpServer::~QtUdpServer() {}