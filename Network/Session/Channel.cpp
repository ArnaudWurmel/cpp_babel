//
// Created by wurmel_a on 30/09/17.
//

#include "Channel.h"

babel::Channel::Channel(std::string const& name) {
    _name = name;
}

std::string const& babel::Channel::getName() const {
    return _name;
}

std::set<std::shared_ptr<babel::User> > const&  babel::Channel::getUserList() const {
    return _memberList;
}

bool    babel::Channel::join(std::shared_ptr<babel::User> const& user) {
    _memberList.insert(user);
    return true;
}

bool    babel::Channel::leave(std::shared_ptr<babel::User> const& user) {
    std::set<std::shared_ptr<babel::User> >::const_iterator it = _memberList.begin();

    while (it != _memberList.end()) {
        if ((*it)->getId() == user->getId()) {
            _memberList.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

void    babel::Channel::triggerChannelEvent(User const &caller, std::string const &body) {
    std::set<std::shared_ptr<babel::User> >::const_iterator it = _memberList.begin();

    while (it != _memberList.end()) {
        if ((*it)->getId() != caller.getId()) {
            (*it)->sendResponse(Message::MessageType::Event, body);
        }
        ++it;
    }
}

babel::Channel::~Channel() {}