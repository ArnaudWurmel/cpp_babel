//
// Server.hh for cpp_babel in /home/vicky/Bureau/EPITECH/cpp_babel/server/headers
// 
// Made by Victorien Fischer
// Login   <victorien.fischer@epitech.eu>
// 
// Started on  Mon Sep 25 06:40:28 2017 Victorien Fischer
// Last update Mon Sep 25 08:46:53 2017 Victorien Fischer
//

#ifndef __BABEL_SERVER__
# define __BABEL_SERVER__

class Server
{
private:
  enum e_instruction
    {
      LIST = 0,
      CALL,
      HANGUP
    };
    
private:
  unsigned short	_port;
  bool			_running;

public:
  Server();
  ~Server();

public:
  unsigned short	getPort() const;
  void			setPort(unsigned short);

public:
  int	run();
};

#endif /* !__BABEL_SERVER__ */
