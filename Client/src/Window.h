#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
# include <QGridLayout>
# include <memory>
#include <mutex>
#include <Socket/Message.h>
#include <queue>
#include <QtWidgets/QPushButton>
#include "../Logger/Logger.h"
#include "User.h"
#include "Channel.h"
#include "SoundManager.h"

class QGroupBox;

class Window : public QWidget, public babel::Logger
{
Q_OBJECT

public:
    Window();
    ~Window();

public:
    void    start();


signals:
    void    WidgetClosed();
    void    userConnected(std::string const&);
    void    userDisconnected(std::string const&);
    void    updateUList(std::vector<std::string> const&);
    void    updateChanList(std::vector<std::string> const&);
    void    newEvent();
    void    joinSuccess(std::vector<std::string> const&);
    void    errored(std::string const&);
    void    joinEvent(std::string const&, std::string const&);
    void    leaveEvent(std::string const&, std::string const&);
    void    joinMeEvent(std::string const&, std::string const&);

public:
    void    closeEvent(QCloseEvent *);
    void    executeAction(babel::Message::MessageType, std::string const& body);

private:
    void    createChannelsList();
    void    createUsersList();

public slots:
    void    updateUserList(std::vector<std::string> const&);
    void    addUser(std::string const&);
    void    removeUser(std::string const&);
    void    updateChannelList(std::vector<std::string> const&);
    void    haveSomethingToClear();
    void    joinThisChannel(std::vector<std::string> const&);
    void    printError(std::string const&);
    void    userQuitChannel(std::string const& username);
    void    userJoinAChannel(std::string const&, std::string const&);
    void    userLeaveAChannel(std::string const&, std::string const&);
    void    userJoinMe(std::string const&, std::string const&);
    void    quitChannel();

private:
    void    keepWriting();

private:
    std::unique_ptr<QGroupBox>  _channelList;
    std::unique_ptr<QGroupBox>  _userList;
    std::unique_ptr<QGridLayout>    _grid;
    std::unique_ptr<SoundManager>   _soundManager;
    std::unique_ptr<QPushButton>    _leaveChannel;
    std::queue<std::pair<babel::Message::MessageType, std::string> > _actionList;
    std::mutex  _commandExecuting;
    std::vector<std::unique_ptr<User> > _uList;
    std::vector<std::unique_ptr<Channel> >  _chanList;
};

#endif