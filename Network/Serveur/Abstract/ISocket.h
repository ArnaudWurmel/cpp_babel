//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKET_H
#define NETWORK_ISOCKET_H

# include <queue>
#include <mutex>
# include "Message/Message.h"

namespace babel {
    class   ISocket {
    public:
        ISocket();
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
    };
}

#endif //NETWORK_ISOCKET_H
