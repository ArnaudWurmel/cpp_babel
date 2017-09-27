//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

# include <boost/asio.hpp>
# include <boost/enable_shared_from_this.hpp>
# include "Message/Message.h"
# include "../Logger/Logger.h"
# include "User.h"

namespace babel {
    class Server;

    class Session : public User, private Logger, public boost::enable_shared_from_this<Session> {
    public:
        Session(boost::asio::io_service&, Server&);
        ~Session();

    };
}

#endif //NETWORK_SESSION_H