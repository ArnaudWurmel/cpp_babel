//
// Created by wurmel_a on 06/10/17.
//

#include <cstring>
#include "QtUdpServer.h"
#include "Message.h"

babel::QtUdpServer::QtUdpServer() {}

void babel::QtUdpServer::bind(std::string const& host, unsigned short port) {
    _socket.bind(QHostAddress(host.c_str()), port);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
}

void    babel::QtUdpServer::stop() {
    _socket.close();
}

void    babel::QtUdpServer::readReady() {
    QHostAddress    sender;
    quint16 port;
    QByteArray  buffer;

    std::cout << "New frame" << std::endl;
    while (this->_socket.hasPendingDatagrams()) {
        buffer.resize(_socket.pendingDatagramSize());
        _socket.readDatagram(buffer.data(), buffer.size(), &sender, &port);
        babel::Message  res;

        if (buffer.size() >= babel::Message::headerSize) {
            std::cout << "New frame" << std::endl;
            std::memcpy(res.data(), buffer.data(), buffer.size());
            if (res.decodeHeader()) {
                std::memcpy(res.getBody(), buffer.data() + babel::Message::headerSize, buffer.size() - babel::Message::headerSize);
                std::cout << res.totalSize() << " " << buffer.size() << std::endl;
                if (res.totalSize() == buffer.size()) {
                    _queueLocker.lock();
                    _frameList.push(std::make_pair(sender.toString().toStdString(), res));
                    _queueLocker.unlock();
                }
                std::cout << "Decoded" << std::endl;
            }
        }
    }
}

void    babel::QtUdpServer::sendFrameTo(std::string const& addr, babel::Message& frame) {
    frame.encodeHeader();
    frame.encodeData();
    std::cout << "Write : " << _socket.writeDatagram(reinterpret_cast<char *>(frame.data()), frame.totalSize(), QHostAddress(addr.c_str()), 8888) << std::endl;
}

bool    babel::QtUdpServer::haveAvailableData() {
    _queueLocker.lock();
    bool state = _frameList.size() > 0;
    _queueLocker.unlock();
    return state;
}

std::pair<std::string, babel::Message>  babel::QtUdpServer::getAvailableData() {
    if (haveAvailableData()) {
        _queueLocker.lock();
        std::pair<std::string, babel::Message>  message = _frameList.front();
        _frameList.pop();
        _queueLocker.unlock();
        return message;
    }
    return std::pair<std::string, babel::Message>();
}

babel::QtUdpServer::~QtUdpServer() {}