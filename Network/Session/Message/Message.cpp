//
// Created by wurmel_a on 25/09/17.
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "Message.h"

babel::Message::Message(MessageType const& type) {
    _message.magicNumber = magic_number;
    _message.body = nullptr;
    _message.bodySize = 0;
    _message.type = type;
}

babel::Message::Message(Message const &other) {
    _message.magicNumber = other._message.magicNumber;
    _message.bodySize = other._message.bodySize;
    _message.type = other._message.type;
    _message.body = new char[_message.bodySize + 1];
    std::memcpy(_message.body, other._message.body, _message.bodySize);
}

char const* babel::Message::getBody() const {
    return _message.body;
}

char    *babel::Message::getBody() {
    return _message.body;
}

void    babel::Message::setBody(const char *body, unsigned int bodySize) {
    if (_message.body) {
        delete[] _message.body;
    }
    _message.bodySize = bodySize;
    _message.body = new char[bodySize + 1];
    std::memcpy(_message.body, body, bodySize);
}

void    *babel::Message::data() {
    return _data;
}

unsigned int    babel::Message::getBodySize() const {
    return _message.bodySize;
}

bool    babel::Message::decodeHeader() {
    std::memcpy(&_message, _data, headerSize);
    if (_message.bodySize > maxBodySize) {
        return false;
    }
    if (_message.body) {
        delete[] _message.body;
    }
    _message.body = new char[_message.bodySize + 1];
    std::memset(_message.body, 0, _message.bodySize + 1);
    return true;
}

void    babel::Message::encodeHeader() {
    std::memcpy(_data, &_message, headerSize);
}

babel::Message::MessageType babel::Message::getType() const {
    return _message.type;
}

babel::Message::~Message() {
    if (_message.body) {
        delete[] _message.body;
    }
}
