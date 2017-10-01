//
// Created by wurmel_a on 01/10/17.
//

#ifndef CLIENT_QTSOCKET_H
#define CLIENT_QTSOCKET_H

# include <QObject>
# include <QTcpSocket>
# include "ISocket.h"

namespace babel {
    class QtSocket : public QObject, public ISocket {
        Q_OBJECT
    public:
        QtSocket(std::mutex&, std::condition_variable&, QObject *parent = 0);
        ~QtSocket();

        virtual void    write(babel::Message message);
        virtual void    close();
        virtual bool    isOpen() const;
        virtual bool    connectSocket(std::string const&, unsigned short);
        virtual void    startSession();
        virtual std::string     getIpAddr() const;

    private slots:
        void    socketConnected();
        void    connectionClosed();
        void    readyRead();

    private:
        std::unique_ptr<QTcpSocket> _socket;
        babel::Message  _readMess;
        bool    _readBody;
    };
}

#endif //CLIENT_QTSOCKET_H
