//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKETACCEPTOR_H
#define NETWORK_ISOCKETACCEPTOR_H

#include <boost/asio/detail/shared_ptr.hpp>
# include "ISocket.h"

namespace babel {
    class   ISocketAcceptor {
    public:
        virtual ~ISocketAcceptor() {}

    public:
        virtual void    startAccept() = 0;
        virtual bool    haveAWaitingClient() = 0;
        virtual std::shared_ptr<babel::ISocket> acceptClient() = 0;
        virtual void    run() = 0;
    };
}

#endif //NETWORK_ISOCKETACCEPTOR_H
