//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKET_H
#define NETWORK_ISOCKET_H

# include <queue>
# include <mutex>
# include <condition_variable>
# include "Message.h"

namespace babel {
    class   ISocket {
    public:
        ISocket(std::mutex&, std::condition_variable&);
        virtual ~ISocket();

        virtual void    write(babel::Message message) = 0;
        virtual void    close() = 0;
        virtual bool    isOpen() const = 0;
        virtual bool    connectSocket(std::string const&, unsigned short port) = 0;
        virtual void    startSession() = 0;
        virtual std::string     getIpAddr() const = 0;
        virtual bool    haveAvailableData();
        babel::Message  getAvailableMessage();

    protected:
        void    addMessage(babel::Message);

    private:
        std::queue<babel::Message>  _messageList;
        std::mutex  _queueLocker;

    protected:
        std::mutex& _haveData;
        std::condition_variable&    _cv;
    };
}

#endif //NETWORK_ISOCKET_H
