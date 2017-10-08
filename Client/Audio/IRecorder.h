//
// Created by wurmel_a on 08/10/17.
//

#ifndef CLIENT_IRECORDER_H
#define CLIENT_IRECORDER_H

#include "IAudio.hpp"
#include "AudioSettings.h"

class   IRecorder : public IAudio {
public:
    virtual ~IRecorder() {}

    virtual DecodedFrame RecordedFrames() = 0;
};

#endif //CLIENT_IRECORDER_H
