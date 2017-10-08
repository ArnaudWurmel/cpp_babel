//
// Created by wurmel_a on 08/10/17.
//

#ifndef CLIENT_IPLAY_H
#define CLIENT_IPLAY_H

#include "IAudio.hpp"
#include "AudioSettings.h"

class   IPlay : public IAudio {
public:
    virtual ~IPlay() {}

    virtual bool PlayFrames(DecodedFrame) = 0;
};

#endif //CLIENT_IPLAY_H
