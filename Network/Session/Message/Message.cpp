//
// Created by wurmel_a on 25/09/17.
//

#include "Message.h"

unsigned int babel::Message::headerSize = (sizeof(AMessage) - sizeof(void *));

babel::Message::Message(MessageType const& type) {
    _message.magicNumber = magic_number;
    _message.body = nullptr;
    _message.bodySize = 0;
    _message.type = type;
}

void const* babel::Message::getBody() const {
    return _message.body;
}

void    *babel::Message::getBody() {
    return _message.body;
}

void    babel::Message::setBody(void *body, unsigned int bodySize) {
    _message.bodySize = bodySize;
    _message.body = body;
}

void    *babel::Message::data() {
    return &_message;
}

bool    babel::Message::validateData() const {
    return _message.magicNumber == magic_number;
}

babel::Message::~Message() {}
