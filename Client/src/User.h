//
// Created by wurmel_a on 03/10/17.
//

#ifndef CLIENT_USER_H
#define CLIENT_USER_H

# include <QLabel>

class User : public QLabel {

public:
    User(std::string const&);
    ~User();

public:
    std::string const&  getName() const;

private:
    std::string _name;
};


#endif //CLIENT_USER_H
