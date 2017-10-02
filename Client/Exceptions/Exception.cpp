//
// Created by wurmel_a on 01/10/17.
//

#include "Exception.h"

babel::Exception::Exception(std::string const& what) {
    _what = what;
}

const char *babel::Exception::what() const noexcept {
    return _what.c_str();
}

babel::Exception::~Exception() {}

babel::NetworkException::NetworkException() : Exception("A network error occured") {}

babel::NetworkException::~NetworkException() {}