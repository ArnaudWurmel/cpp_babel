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
            Unknown = 0,
            Connect = 1,
            Error = 2,
            Userlist = 3,
            ChannelList = 4,
            Join = 5,
            Event = 6,
            Leave = 7,
            CreateChannel = 8,
            Audio = 9
        };

#include "packed.h"
        struct AMessage {
            unsigned short magicNumber;
            MessageType type;
            unsigned int bodySize;
            char    *body;
        } PACKED;
#include "endpacked.h"

        enum { headerSize = sizeof(unsigned short) + sizeof(MessageType) + sizeof(unsigned int) };
        enum { maxBodySize = 512 };
        enum { magic_number = 0x424D };

    public:
        Message();
        Message(MessageType const&);
        Message(Message const&);
        ~Message();

    public:
        void    setBody(const char *, unsigned int);
        void    setBody(const unsigned char *, unsigned int);
        char    *getBody();
        char const* getBody() const;
        char    *data();
        bool    decodeHeader();
        bool    encodeHeader();
        void    encodeData();
        unsigned int    getBodySize() const;
        unsigned int    totalSize() const;
        MessageType getType() const;
        void    setType(MessageType);

    private:
        unsigned int    _bodySize;
        AMessage    _message;
        std::vector<char>   _data;
    };
}

#endif //NETWORK_MESSAGE_H