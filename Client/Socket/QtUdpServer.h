//
// Created by wurmel_a on 06/10/17.
//

#ifndef CLIENT_QTUDPSERVER_H
#define CLIENT_QTUDPSERVER_H

# include <QUdpSocket>
#include <thread>
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
        void    runThread();
        void    stop();
        void    sendFrameTo(std::string const&, babel::Message&);

    private:
        void    sendLoop();

    public slots:
        void    readReady();

    private:
        QUdpSocket  _socket;
        babel::Message  _inMess;
        bool    _readHeader;
        bool    _continue;
        std::unique_ptr<std::thread>    _thread;
        std::vector<std::pair<std::string, babel::Message> >    _frameList;
    };
}

#endif //CLIENT_QTUDPSERVER_H
