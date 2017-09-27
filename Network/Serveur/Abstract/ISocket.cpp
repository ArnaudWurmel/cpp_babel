//
// Created by wurmel_a on 27/09/17.
//

#include <exception>
#include "ISocket.h"

babel::ISocket::ISocket() {}

bool babel::ISocket::haveAvailableData() {
    _queueLocker.lock();
    bool state = _messageList.size() > 0;
    _queueLocker.unlock();
    return state;
}

void    babel::ISocket::addMessage(babel::Message message) {
    _queueLocker.lock();
    _messageList.push(message);
    _queueLocker.unlock();
}

babel::Message  babel::ISocket::getAvailableMessage() {
    _queueLocker.lock();
    if (_messageList.size() > 0) {
        babel::Message message = _messageList.front();
        _messageList.pop();
        _queueLocker.unlock();
        return message;
    }
    throw std::exception();
}

babel::ISocket::~ISocket() {}