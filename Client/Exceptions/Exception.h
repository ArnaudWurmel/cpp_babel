//
// Created by wurmel_a on 01/10/17.
//

#ifndef CLIENT_EXCEPTION_H
#define CLIENT_EXCEPTION_H

# include <iostream>
# include <exception>

namespace babel {
    class Exception : public std::exception {
    public:
        Exception(std::string const&);
        ~Exception();

    public:
        char const *what() const noexcept;

    private:
        std::string _what;
    };

    class   NetworkException : public Exception {
    public:
        NetworkException();
        ~NetworkException();
    };
}

#endif //CLIENT_EXCEPTION_H
