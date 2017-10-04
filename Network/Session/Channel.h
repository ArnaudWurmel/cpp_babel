//
// Created by wurmel_a on 30/09/17.
//

#ifndef NETWORK_CHANNEL_H
#define NETWORK_CHANNEL_H

# include <iostream>
# include <set>
# include "User.h"

namespace   babel {
    class Channel {
    public:
        Channel(std::string const&);
        ~Channel();

    public:
        std::string const& getName() const;
        std::set<std::shared_ptr<babel::User> > const&  getUserList() const;
        bool    join(std::shared_ptr<babel::User> const&);
        bool    leave(std::shared_ptr<babel::User> const&);
        void    triggerChannelEvent(User const& caller, std::string const& body);

    private:
        std::string _name;
        std::set<std::shared_ptr<babel::User> > _memberList;
    };
}

#endif //NETWORK_CHANNEL_H
