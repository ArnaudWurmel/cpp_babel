//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <vector>

namespace babel {
    class Message {
    public:
        enum    MessageType {
            Unknown,
            Connect,
            Error
        };

        struct __attribute__((packed)) AMessage {
            unsigned short magicNumber;
            MessageType type;
            unsigned int bodySize;
            char    *body;
        };

        enum { headerSize = sizeof(unsigned short) + sizeof(MessageType) + sizeof(unsigned int) };
        enum { maxBodySize = 512 };
        enum { magic_number = 0x424D };

    public:
        Message(MessageType const&);
        Message(Message const&);
        ~Message();

    public:
        void    setBody(const char *, unsigned int);
        char    *getBody();
        char const* getBody() const;
        void    *data();
        bool    decodeHeader();
        void    encodeHeader();
        void    encodeData();
        unsigned int    getBodySize() const;
        unsigned int    totalSize() const;
        MessageType getType() const;

    private:
        unsigned int    _bodySize;
        AMessage    _message;
        std::vector<char>   _data;
    };
}

#endif //NETWORK_MESSAGE_H
