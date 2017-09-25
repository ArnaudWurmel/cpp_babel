//
// Client.cpp for cpp_babel in /home/vicky/Bureau/EPITECH/cpp_babel/server
// 
// Made by Victorien Fischer
// Login   <victorien.fischer@epitech.eu>
// 
// Started on  Mon Sep 25 09:02:29 2017 Victorien Fischer
// Last update Mon Sep 25 09:18:33 2017 Victorien Fischer
//

#include "Client.hh"
#include <ctime>
#include <string>

/*
** =====================================================
** OBJECT FUNCTIONS
** =====================================================
*/
/*
** Constructor
*/
Client::Client(unsigned int id) : _id(id), _ip("N/A"), _connectTime(std::time(nullptr))
{
}

/*
** Copy Constructor
*/
Client::Client(const Client &other) : _id(other.getID()), _ip(other.getIP()), _connectTime(other.getConnectTime())
{
}

/*
** Operator =
*/
Client&	Client::operator=(const Client &other)
{
  if (&other != this)
    {
      _id = other.getID();
      _ip = other.getIP();
      _connectTime = other.getConnectTime();
    }
  return (*this);
}

/*
** Destructor
*/
Client::~Client()
{
}

/*
** =====================================================
** GETTER
** =====================================================
*/
/*
** Get ID
*/
unsigned int	Client::getID() const
{
  return (_id);
}

/*
** Get IP
*/
std::string	Client::getIP() const
{
  return (_ip);
}

/*
** Get ConnectTime
*/
std::time_t	Client::getConnectTime() const
{
  return (_connectTime);
}

/*
** =====================================================
** SETTER
** =====================================================
*/
/*
** Set IP
*/
void	Client::setIP(std::string ip)
{
  _ip = ip;
}
