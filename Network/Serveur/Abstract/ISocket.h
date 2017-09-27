//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKET_H
#define NETWORK_ISOCKET_H

# include <queue>
# include <mutex>
# include <condition_variable>
# include "Message/Message.h"

namespace babel {
    class   ISocket {
    public:
        ISocket(std::mutex&, std::condition_variable&);
        virtual ~ISocket();

        virtual bool    connectSocket() = 0;
        virtual void    startSession() = 0;
        virtual bool    haveAvailableData();
        babel::Message  getAvailableMessage();

    protected:
        void    addMessage(babel::Message);

    private:
        std::queue<babel::Message>  _messageList;
        std::mutex  _queueLocker;

    private:
        std::mutex& _haveData;
        std::condition_variable&    _cv;
    };
}

#endif //NETWORK_ISOCKET_H
