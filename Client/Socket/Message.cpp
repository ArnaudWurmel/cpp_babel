//
// Created by wurmel_a on 25/09/17.
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "Message.h"

babel::Message::Message() : _data(headerSize + maxBodySize + 1, 0) {
    std::memset(&_message, 0, sizeof(_message));
    _message.magicNumber = magic_number;
    _message.body = nullptr;
    _message.bodySize = 0;
    _message.type = babel::Message::MessageType::Unknown;
}

babel::Message::Message(MessageType const& type) : _data(headerSize + maxBodySize + 1, 0) {
    std::memset(&_message, 0, sizeof(_message));
    _message.magicNumber = magic_number;
    _message.body = nullptr;
    _message.bodySize = 0;
    _message.type = type;
}

babel::Message::Message(Message const&other) : _data(headerSize + maxBodySize + 1, 0) {
    _message.magicNumber = other._message.magicNumber;
    _message.bodySize = other._message.bodySize;
    _message.type = other._message.type;
    _message.body = new char[_message.bodySize];
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
    _message.body = new char[bodySize];
    std::memcpy(_message.body, body, bodySize);
}

char    *babel::Message::data() {
    return _data.data();
}

unsigned int    babel::Message::getBodySize() const {
    return _message.bodySize;
}

bool    babel::Message::decodeHeader() {
    std::memcpy(&_message, _data.data(), headerSize);
    if (_message.bodySize > maxBodySize) {
        return false;
    }
    if (_message.body) {
        delete[] _message.body;
    }
    _message.body = new char[_message.bodySize];
    std::memset(_message.body, 0, _message.bodySize);
    return true;
}

void    babel::Message::setType(MessageType type) {
    _message.type = type;
}

void    babel::Message::encodeData() {
    if (_message.bodySize >= maxBodySize) {
        _message.bodySize = maxBodySize - 1;
    }
    std::memcpy(_data.data() + headerSize, _message.body, _message.bodySize);
}

bool    babel::Message::encodeHeader() {
    if (_message.bodySize < maxBodySize)
        std::memcpy(_data.data(), &_message, headerSize);
    return _message.bodySize < maxBodySize;
}

babel::Message::MessageType babel::Message::getType() const {
    return _message.type;
}

unsigned int    babel::Message::totalSize() const {
    return getBodySize() + headerSize;
}

babel::Message::~Message() {
    if (_message.body) {
        delete[] _message.body;
    }
}
