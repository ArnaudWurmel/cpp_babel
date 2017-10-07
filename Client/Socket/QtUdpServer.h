//
// Created by wurmel_a on 06/10/17.
//

#ifndef CLIENT_QTUDPSERVER_H
#define CLIENT_QTUDPSERVER_H

# include <QUdpSocket>
#include <thread>
#include <mutex>
#include <queue>
# include "IServer.h"
#include "Message.h"

namespace babel {
    class QtUdpServer : public QObject, public IServer {
        Q_OBJECT

    public:
        QtUdpServer();
        ~QtUdpServer();

    public:
        void    bind(std::string const&, unsigned short);
        void    stop();
        void    sendFrameTo(std::string const&, babel::Message&);
        bool    haveAvailableData();
        std::pair<std::string, babel::Message>  getAvailableData();

    public slots:
        void    readReady();

    private:
        std::mutex  _queueLocker;
        QUdpSocket  _socket;
        std::queue<std::pair<std::string, babel::Message> >    _frameList;
    };
}

#endif //CLIENT_QTUDPSERVER_H
