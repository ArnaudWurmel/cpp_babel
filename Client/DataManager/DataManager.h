//
// Created by wurmel_a on 01/10/17.
//

#ifndef CLIENT_DATAMANAGER_H
#define CLIENT_DATAMANAGER_H

#include <map>
#include <thread>
#include <src/Window.h>
# include "../Socket/ISocket.h"
# include "../Logger/Logger.h"

namespace babel {
    class DataManager : private babel::Logger {

    public:
        static DataManager *getInstance(DataManager *inst = NULL);

    public:
        DataManager(Window&, std::string const&, unsigned short port);
        ~DataManager();

    public:
        std::string const&  getUsername() const;
        std::string const&  getLastError() const;
        std::string const&  getCurrentChannel() const;
        void    setCurrentChannelName(std::string const&);

    public:
        void    executeAction(babel::Message::MessageType type, std::string const &body);
        void    startData();
        void    clearEventList();

    private:
        void    senderLoop();
        void    handleConnect(babel::Message const&);
        void    handleError(babel::Message const&);
        void    handleUserList(babel::Message const&);
        void    handleChannelList(babel::Message const&);
        void    handleJoin(babel::Message const&);

        /*
         * Events
         */
        void    handleCon(std::vector<std::string> const&);
        void    handleDisc(std::vector<std::string> const&);
        void    handleUserJoin(std::vector<std::string> const&);
        void    handleUserLeave(std::vector<std::string> const&);

    public:
        static std::vector<std::string>    getTokenFrom(std::string const&, std::string const& sep);

    private:
        bool    _haveInput;
        Window  &_window;
        std::queue<babel::Message>  _input;
        std::mutex  _synchroniser;
        std::mutex  _socketReading;
        std::condition_variable _socketWaiter;
        std::mutex  _lockInput;
        std::condition_variable _inputWaiter;
        bool    _continue;
        std::map<babel::Message::MessageType, void (babel::DataManager::*)(babel::Message const&)>    _functionPtrs;
        std::shared_ptr<ISocket>    _socket;
        babel::Message  _sender;
        std::unique_ptr<std::thread>    _dataLooper;
        std::map<std::string, void (babel::DataManager::*)(std::vector<std::string> const&) >    _eventPtrs;

    private:
        std::string _username;
        std::string _lastError;
        std::string _channelName;
        std::queue<babel::Message >    _eventList;
    };
}

#endif //CLIENT_DATAMANAGER_H
