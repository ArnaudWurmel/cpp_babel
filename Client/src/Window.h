#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
# include <QGridLayout>
# include <memory>
#include <mutex>
#include <Socket/Message.h>
#include <queue>

class QGroupBox;

class Window : public QWidget
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

protected:
    void    closeEvent(QCloseEvent *);
    void    executeAction(babel::Message::MessageType, std::string const& body);

private:
    void    createChannelsList();
    void    createUsersList();

public slots:
    void    updateUserList(std::vector<std::string> const&);
    void    addUser(std::string const&);

private:
    std::unique_ptr<QGroupBox>  _channelList;
    std::unique_ptr<QGroupBox>  _userList;
    std::unique_ptr<QGridLayout>    _grid;
    std::queue<std::pair<babel::Message::MessageType, std::string> > _actionList;
    std::mutex  _commandExecuting;
};

#endif