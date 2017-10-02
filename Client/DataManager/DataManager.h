//
// Created by wurmel_a on 01/10/17.
//

#ifndef CLIENT_DATAMANAGER_H
#define CLIENT_DATAMANAGER_H

#include <map>
# include "../Socket/ISocket.h"
# include "../Logger/Logger.h"

namespace babel {
    class DataManager : private babel::Logger {
    public:
        DataManager(std::string const&, unsigned short port);
        ~DataManager();

    public:
        std::string const&  getUsername() const;
        std::string const&  getLastError() const;

    public:
        bool    connectUser(std::string const& name = "");

    private:
        bool    waitForCommand(babel::Message::MessageType);
        bool    emptyCommandList();

    private:
        void    handleConnect(babel::Message const&);

    private:
        std::map<babel::Message::MessageType, void (babel::DataManager::*)(babel::Message const&)>    _functionPtrs;
        std::string _username;
        std::shared_ptr<ISocket>    _socket;
        std::mutex  _locker;
        std::condition_variable _cv;
        babel::Message  _sender;
        std::queue<babel::Message >    _eventList;
        std::string _lastError;
    };
}

#endif //CLIENT_DATAMANAGER_H
