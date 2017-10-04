#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
# include <QGridLayout>
# include <memory>
#include <mutex>
#include <Socket/Message.h>
#include <queue>
#include "../Logger/Logger.h"
#include "User.h"

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
    void    updateUList(std::vector<std::string> const&);
    void    newEvent();

protected:
    void    closeEvent(QCloseEvent *);
    void    executeAction(babel::Message::MessageType, std::string const& body);

private:
    void    createChannelsList();
    void    createUsersList();

public slots:
    void    updateUserList(std::vector<std::string> const&);
    void    addUser(std::string const&);
    void    haveSomethingToClear();

private:
    std::unique_ptr<QGroupBox>  _channelList;
    std::unique_ptr<QGroupBox>  _userList;
    std::unique_ptr<QGridLayout>    _grid;
    std::queue<std::pair<babel::Message::MessageType, std::string> > _actionList;
    std::mutex  _commandExecuting;
    std::vector<std::shared_ptr<User> > _uList;
};

#endif