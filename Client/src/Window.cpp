#include <QGridLayout>
#include <QRadioButton>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QGroupBox>
#include <QListView>
#include <iostream>
#include "./Window.h"
#include "../DataManager/DataManager.h"
#include "Channel.h"

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
}

void    Window::start() {
    executeAction(babel::Message::MessageType::Connect, "");
    executeAction(babel::Message::MessageType::Userlist, "");
}

void    Window::createChannelsList()
{
    _channelList = std::unique_ptr<QGroupBox>(new QGroupBox(tr("Channels list")));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch(1);
    vbox->addWidget(new Channel("Arnaud"));
    _channelList->setLayout(vbox);
}

void    Window::executeAction(babel::Message::MessageType type, std::string const& body) {
    bool    onWriting = _actionList.size() > 0;

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
    say("Added new user <" + username + ">"
                                               "");
    _uList.push_back(std::shared_ptr<User>(new User(username)));
    _userList->layout()->addWidget(_uList.back().get());
    babel::DataManager::getInstance()->clearEventList();
    if (_actionList.size()) {
        _actionList.pop();
        if (_actionList.size()) {
            babel::DataManager::getInstance()->executeAction(_actionList.front().first, _actionList.front().second);
        }
    }
}

void    Window::updateUserList(std::vector<std::string> const& userList) {
    std::vector<std::string>::const_iterator    iterator = userList.begin();

    while (iterator != userList.end()) {
        std::vector<std::shared_ptr<User> >::const_iterator itU = _uList.begin();
        bool    found = false;

        while (!found && itU != _uList.end()) {
            if ((*itU)->getName().compare(*iterator) == 0) {
                found = true;
            }
            ++itU;
        }
        if (!found)
            addUser(*iterator);
        ++iterator;
    }
    babel::DataManager::getInstance()->clearEventList();
    if (_actionList.size()) {
        _actionList.pop();
        if (_actionList.size()) {
            babel::DataManager::getInstance()->executeAction(_actionList.front().first, _actionList.front().second);
        }
    }
}

void    Window::haveSomethingToClear() {
    std::cout << "Something to clear" << std::endl;
    babel::DataManager::getInstance()->clearEventList();
}

Window::~Window() {}
