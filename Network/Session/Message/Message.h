//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

namespace babel {
    class Message {
    public:
        static unsigned int headerSize;

        enum { magic_number = 0x424D };

        enum    MessageType {
            Unknown,
            Connect
        };

        struct __attribute__((packed)) AMessage {
            unsigned short magicNumber;
            MessageType type;
            unsigned int bodySize;
            void    *body;
        };

    public:
        Message(MessageType const&);
        ~Message();

    public:
        void    setBody(void *, unsigned int);
        void    *getBody();
        void const* getBody() const;
        void    *data();
        bool    validateData() const;
        unsigned int getBodySize() const;

    private:
        AMessage    _message;
    };
}

#endif //NETWORK_MESSAGE_H
