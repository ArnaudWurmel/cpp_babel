//
// Created by wurmel_a on 03/10/17.
//

#include <iostream>
#include <DataManager/DataManager.h>
#include "Channel.h"

Channel::Channel(std::string const& name) : QGroupBox(name.c_str()), _name(name) {
    _layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    _layout->addStretch(1);
    setLayout(_layout.get());
}

void    Channel::addUser(std::shared_ptr<User>& newUser) {
    _userList.push_back(newUser);
    _layout->addWidget(_userList.back().get());
}

std::string const&  Channel::getName() const {
    return _name;
}

void    Channel::mousePressEvent(QMouseEvent *) {
    std::cout << "Mouse pressed" << std::endl;
    babel::DataManager::getInstance()->executeAction(babel::Message::MessageType::Unknown, "");
}

Channel::~Channel() {}