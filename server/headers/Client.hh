//
// Client.hh for cpp_babel in /home/vicky/Bureau/EPITECH/cpp_babel/server
// 
// Made by Victorien Fischer
// Login   <victorien.fischer@epitech.eu>
// 
// Started on  Mon Sep 25 08:52:44 2017 Victorien Fischer
// Last update Mon Sep 25 09:06:34 2017 Victorien Fischer
//

#ifndef __BABEL_CLIENT__
# define __BABEL_CLIENT__

#include <ctime>
#include <string>

class Client
{
private:
  unsigned int		_id;
  std::string		_ip;
  std::time_t		_connectTime;

public:
  Client(unsigned int);
  ~Client();

public:
  Client(const Client &);
  Client	&operator=(const Client &);

public:
  unsigned int	getID() const;
  std::string	getIP() const;
  std::time_t	getConnectTime() const;

public:
  void		setIP(std::string);
};

#endif /* !__BABEL_CLIENT__ */
