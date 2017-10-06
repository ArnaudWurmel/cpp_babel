//
// Created by wurmel_a on 03/10/17.
//

#ifndef CLIENT_CHANNEL_H
#define CLIENT_CHANNEL_H

# include <QGroupBox>
# include <QVBoxLayout>
# include <memory>
# include "User.h"

class   Window;

class Channel : public QGroupBox {

public:
    Channel(Window&, std::string const&);
    ~Channel();

public:
    void    addUser(std::string const&);
    void    removeUser(std::string const&);
    std::string const&  getName() const;

protected:
    void    mousePressEvent(QMouseEvent *);

private:
    Window& _window;
    std::vector<std::unique_ptr<User> > _userList;
    std::unique_ptr<QVBoxLayout>    _layout;
    std::string _name;
};


#endif //CLIENT_CHANNEL_H
