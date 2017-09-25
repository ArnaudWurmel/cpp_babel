//
// Server.cpp for cpp_babel in /home/vicky/Bureau/EPITECH/cpp_babel/server/sources
// 
// Made by Victorien Fischer
// Login   <victorien.fischer@epitech.eu>
// 
// Started on  Mon Sep 25 06:58:39 2017 Victorien Fischer
// Last update Mon Sep 25 08:27:14 2017 Victorien Fischer
//

#include "Server.hh"


/*
** ==============================================================
** OBJECT FUNCTIONS
** ==============================================================
*/
/*
** Constructor
*/
Server::Server()
{
}

/*
** Destructor
*/
Server::~Server()
{
}

/*
** ==============================================================
** SETTER
** ==============================================================
*/
void	Server::setPort(unsigned short port)
{
  _port = port;
}

/*
** ==============================================================
** GETTER
** ==============================================================
*/
unsigned short	Server::getPort() const
{
  return (_port);
}

/*
** ==============================================================
** FUNCTIONS
** ==============================================================
*/
int	Server::run()
{
  _running = 1;
  while (_running)
    {
    }
  return (0);
}

