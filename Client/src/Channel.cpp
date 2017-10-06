//
// Created by wurmel_a on 03/10/17.
//

#include <iostream>
#include <DataManager/DataManager.h>
#include "Channel.h"
#include "Window.h"

Channel::Channel(Window& window, std::string const& name) : QGroupBox(name.c_str()), _window(window), _name(name) {
    _layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    _layout->addStretch(1);
    setLayout(_layout.get());
}

void    Channel::addUser(std::string const& newUser) {
    _userList.push_back(std::unique_ptr<User>(new User(newUser)));
    _layout->addWidget(_userList.back().get());
}

std::string const&  Channel::getName() const {
    return _name;
}

void    Channel::mousePressEvent(QMouseEvent *) {
    babel::DataManager::getInstance()->setCurrentChannelName(getName());
    _window.executeAction(babel::Message::MessageType::Join, getName());
}

void    Channel::removeUser(std::string const& user) {
    std::vector<std::unique_ptr<User> >::const_iterator it = _userList.begin();

    while (it != _userList.end()) {
        if ((*it)->getName().compare(user) == 0) {
            _layout->removeWidget((*it).get());
            _userList.erase(it);
            return ;
        }
        ++it;
    }
}

Channel::~Channel() {
    _userList.clear();
}