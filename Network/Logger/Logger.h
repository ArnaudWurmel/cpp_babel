//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_LOGGER_H
#define NETWORK_LOGGER_H

# include <string>

namespace babel {
    class Logger {
    public:
        Logger(std::string const& className);
        virtual ~Logger();

    protected:
        void    say(std::string const&);

    private:
        std::string _className;
    };
}

#endif //NETWORK_LOGGER_H
