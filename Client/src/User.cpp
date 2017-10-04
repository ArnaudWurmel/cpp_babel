//
// Created by wurmel_a on 03/10/17.
//

#include "User.h"

User::User(std::string const& name) : QLabel(name.c_str()), _name(name) {

}

std::string const&  User::getName() const {
    return _name;
}

User::~User() {}