//
// Created by wurmel_a on 03/10/17.
//

#ifndef CLIENT_CHANNEL_H
#define CLIENT_CHANNEL_H

# include <QGroupBox>
# include <QVBoxLayout>
# include <memory>
# include "User.h"

class Channel : public QGroupBox {

public:
    Channel(std::string const&);
    ~Channel();

public:
    void    addUser(std::shared_ptr<User>&);
    std::string const&  getName() const;

private:
    std::vector<std::shared_ptr<User> > _userList;
    std::unique_ptr<QVBoxLayout>    _layout;
    std::string _name;
};


#endif //CLIENT_CHANNEL_H
