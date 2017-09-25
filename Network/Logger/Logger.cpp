//
// Created by wurmel_a on 25/09/17.
//

#include <iostream>
#include "Logger.h"

babel::Logger::Logger(std::string const &className) {
    _className = className;
}

void babel::Logger::say(std::string const& message) {
    std::cout << "[" << _className << "] " << message << std::endl;
}

babel::Logger::~Logger() {}