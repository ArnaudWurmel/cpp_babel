//
// Created by wurmel_a on 06/10/17.
//

#include "QtUdpServer.h"
#include "Message.h"

babel::QtUdpServer::QtUdpServer() {
    _continue = true;
}

void babel::QtUdpServer::bind(std::string const& host, unsigned short port) {
    _readHeader = true;
    _socket.bind(QHostAddress(host.c_str()), port);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
}

void    babel::QtUdpServer::runThread() {
    std::cout << " ================ INITIALIZED ============== " << std::endl;
    _thread = std::unique_ptr<std::thread>(new std::thread(&babel::QtUdpServer::sendLoop, this));
}

void    babel::QtUdpServer::stop() {
    _continue = false;
    _thread->join();
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
        _frameList.push_back(std::make_pair(sender.toString().toStdString(), _inMess));
        _readHeader = false;
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

void    babel::QtUdpServer::sendLoop() {
    while (_continue) {
    }
}

babel::QtUdpServer::~QtUdpServer() {
    if (_continue)
        stop();
}