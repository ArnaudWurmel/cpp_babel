//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

# include <set>
# include <string>
# include <boost/bind.hpp>
# include <boost/asio.hpp>
# include <thread>
# include <condition_variable>
# include "../Logger/Logger.h"
# include "Abstract/BoostAcceptor.h"
# include "Abstract/BoostSocket.h"
# include "Abstract/ISocketAcceptor.h"
# include "../Session/User.h"
# include "../Session/Channel.h"

namespace   babel {
    class Server : private Logger {
    public:
        explicit Server(unsigned int port);
        ~Server();

    public:
        void    connectUser(babel::User&, babel::Message&);
        void    userList(babel::User&, babel::Message&);
        void    channelList(babel::User&, babel::Message&);
        void    joinChannel(babel::User&, babel::Message&);
        void    leaveChannelFor(babel::User&, babel::Message&);
        void    createChannel(babel::User&, babel::Message&);

    public:
        void    leaveChannel(babel::User&);

    private:
        void    threadLoop();
        void    triggerEvent(unsigned int senderId, std::string const&);
        bool    validateName(std::string const&);

    private:
        std::unique_ptr<babel::ISocketAcceptor> _socketAcceptor;
        std::unique_ptr<std::thread>    _acceptorThread;
        bool    _threadRunning;
        std::mutex  _haveAction;
        std::condition_variable _cv;
        std::set<std::shared_ptr<User> >    _userList;
        std::set<std::unique_ptr<Channel> >    _channelList;
    };
}

#endif //NETWORK_SERVER_H