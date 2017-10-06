#include <QGridLayout>
#include <QRadioButton>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QGroupBox>
#include <QListView>
#include <QTimer>
#include <iostream>
#include "./Window.h"
#include "../DataManager/DataManager.h"

Window::Window() : Logger("Window")
{
    createChannelsList();
    createUsersList();
    _grid = std::unique_ptr<QGridLayout>(new QGridLayout);
    _grid->addWidget(_channelList.get(), 0, 0);
    _grid->addWidget(_userList.get(), 0, 1);
    setLayout(_grid.get());
    setWindowTitle(tr("cpp_babel"));
    resize(480, 320);
    connect(this, SIGNAL(userConnected(std::string const&)), SLOT(addUser(std::string const&)));
    connect(this, SIGNAL(updateUList(std::vector<std::string> const&)), SLOT(updateUserList(std::vector<std::string> const&)));
    connect(this, SIGNAL(newEvent()), SLOT(haveSomethingToClear()));
    connect(this, SIGNAL(userDisconnected(std::string const&)), SLOT(removeUser(std::string const&)));
    connect(this, SIGNAL(updateChanList(std::vector<std::string> const&)), SLOT(updateChannelList(std::vector<std::string> const&)));
    connect(this, SIGNAL(joinSuccess(std::vector<std::string> const&)), SLOT(joinThisChannel(std::vector<std::string> const&)));
    connect(this, SIGNAL(errored(std::string const&)), SLOT(printError(std::string const&)));
    connect(this, SIGNAL(joinEvent(std::string const&, std::string const&)), SLOT(userJoinAChannel(std::string const&, std::string const&)));
    connect(this, SIGNAL(leaveEvent(std::string const&, std::string const&)), SLOT(userLeaveAChannel(std::string const&, std::string const&)));
    connect(this, SIGNAL(joinMeEvent(std::string const&, std::string const&)), SLOT(userJoinMe(std::string const&, std::string const&)));
    _soundManager = std::unique_ptr<SoundManager>(new SoundManager());
}

void    Window::start() {
    executeAction(babel::Message::MessageType::Connect, "");
    executeAction(babel::Message::MessageType::Userlist, "");
    executeAction(babel::Message::MessageType::ChannelList, "");
}

void    Window::createChannelsList()
{
    _channelList = std::unique_ptr<QGroupBox>(new QGroupBox(tr("Channels list")));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch(1);
    _channelList->setLayout(vbox);
}

void    Window::executeAction(babel::Message::MessageType type, std::string const& body) {
    bool    onWriting = _actionList.size() > 0;

    say(std::string("Add action : ") + std::to_string(type) + " " + body);
    _actionList.push(std::make_pair(type, body));
    if (!onWriting) {
        babel::DataManager::getInstance()->executeAction(_actionList.front().first, _actionList.front().second);
    }
}

void    Window::closeEvent(QCloseEvent *event) {
    event->accept();
    QCoreApplication::quit();
}

void Window::createUsersList()
{
    _userList = std::unique_ptr<QGroupBox>(new QGroupBox("Users list"));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch(1);
    _userList->setLayout(vbox);
}

void    Window::addUser(std::string const& username) {
    say("Added new user <" + username + ">");
    _uList.push_back(std::unique_ptr<User>(new User(username)));
    _userList->layout()->addWidget(_uList.back().get());
    keepWriting();
}

void    Window::updateUserList(std::vector<std::string> const& userList) {
    std::vector<std::string>::const_iterator    iterator = userList.begin();

    while (iterator != userList.end()) {
        std::vector<std::unique_ptr<User> >::const_iterator itU = _uList.begin();
        bool    found = false;

        while (!found && itU != _uList.end()) {
            if ((*itU)->getName().compare(*iterator) == 0) {
                found = true;
            }
            ++itU;
        }
        if (!found) {
            _uList.push_back(std::unique_ptr<User>(new User(*iterator)));
            _userList->layout()->addWidget(_uList.back().get());
        }
        ++iterator;
    }
    keepWriting();
}

void    Window::removeUser(std::string const& username) {
    std::vector<std::unique_ptr<User> >::const_iterator it = _uList.begin();
    bool    found = false;

    while (it != _uList.end() && !found) {
        if ((*it)->getName().compare(username) == 0) {
            found = true;
        }
        else
            ++it;
    }
    if (!found)
        return ;
    _userList->layout()->removeWidget((*it).get());
    std::vector<std::unique_ptr<Channel> >::iterator    itChan = _chanList.begin();

    while (itChan != _chanList.end()) {
        (*itChan)->removeUser((*it)->getName());
        ++itChan;
    }
    _uList.erase(it);
}

void    Window::updateChannelList(std::vector<std::string> const& tokenList) {
    std::vector<std::string>::const_iterator    it = tokenList.begin();

    while (it != tokenList.end()) {
        std::cout << "Channel : " << *it << std::endl;
        std::vector<std::string>    channelInfo = babel::DataManager::getTokenFrom(*it, ",");
        std::vector<std::unique_ptr<Channel> >::const_iterator  itC = _chanList.begin();
        bool    found = false;

        if (channelInfo.size() >= 1) {
            while (!found && itC != _chanList.end()) {
                if ((*itC)->getName().compare(*it) == 0) {
                    found = true;
                }
                ++itC;
            }
            if (!found) {
                _chanList.push_back(std::unique_ptr<Channel>(new Channel(*this, channelInfo[0])));
                _channelList->layout()->addWidget(_chanList.back().get());
                std::vector<std::string>::const_iterator    itUserTakingPart = channelInfo.begin() + 1;

                while (itUserTakingPart != channelInfo.end()) {
                    _chanList.back()->addUser(*itUserTakingPart);
                    ++itUserTakingPart;
                }
            }
        }
        ++it;
    }
    keepWriting();
}

void    Window::keepWriting() {
    babel::DataManager::getInstance()->clearEventList();
    if (_actionList.size()) {
        _actionList.pop();
        if (_actionList.size()) {
            babel::DataManager::getInstance()->executeAction(_actionList.front().first, _actionList.front().second);
        }
    }
}

void    Window::printError(std::string const& error) {
    say(error);
    keepWriting();
}

void    Window::joinThisChannel(std::vector<std::string> const& userList) {
    userQuitChannel(babel::DataManager::getInstance()->getUsername());
    std::string channelName = babel::DataManager::getInstance()->getCurrentChannel();
    std::vector<std::unique_ptr<Channel> >::iterator    it = _chanList.begin();

    while (it != _chanList.end()) {
        if ((*it)->getName().compare(channelName) == 0) {
            std::vector<std::unique_ptr<User> >::iterator iterator = _uList.begin();

            while (iterator != _uList.end()) {
                if ((*iterator)->getName().compare(babel::DataManager::getInstance()->getUsername()) == 0) {
                    if (_soundManager->isRunning())
                        _soundManager->closeServer();
                    _soundManager->openServer(channelName);
                    std::vector<std::string>::const_iterator itUList = userList.begin();

                    while (itUList != userList.end()) {
                        _soundManager->addUser(*itUList);
                        ++itUList;
                    }
                    (*it)->addUser((*iterator)->getName());
                    keepWriting();
                    return ;
                }
                ++iterator;
            }
        }
        ++it;
    }
    keepWriting();
}

void    Window::userQuitChannel(std::string const &username) {
    std::vector<std::unique_ptr<User> >::const_iterator it = _uList.begin();

    while (it != _uList.end()) {
        if ((*it)->getName().compare(username) == 0) {
            std::vector<std::unique_ptr<Channel> >::iterator    itChan = _chanList.begin();

            if (_soundManager->isRunning()) {
                _soundManager->closeServer();
            }
            while (itChan != _chanList.end()) {
                (*itChan)->removeUser((*it)->getName());
                ++itChan;
            }
            return ;
        }
        ++it;
    }
}

void    Window::userJoinAChannel(std::string const& username, std::string const& channelName) {
    std::vector<std::unique_ptr<Channel> >::iterator    it = _chanList.begin();

    while (it != _chanList.end()) {
        if ((*it)->getName().compare(channelName) == 0) {
            (*it)->addUser(username);
            return ;
        }
        ++it;
    }
}

void    Window::userLeaveAChannel(std::string const& username, std::string const& channelName) {
    std::vector<std::unique_ptr<Channel> >::iterator    it = _chanList.begin();

    while (it != _chanList.end()) {
        if ((*it)->getName().compare(channelName) == 0) {
            _soundManager->stopPlayingAUser(username);
            (*it)->removeUser(username);
            return ;
        }
        ++it;
    }
}

void    Window::userJoinMe(std::string const& username, std::string const& ipAddr) {
    _soundManager->addUser(username + " " + ipAddr);
}

void    Window::haveSomethingToClear() {
    babel::DataManager::getInstance()->clearEventList();
}

Window::~Window() {
    _grid->removeWidget(_userList.get());
    _grid->removeWidget(_channelList.get());
    _chanList.clear();
    _uList.clear();
}