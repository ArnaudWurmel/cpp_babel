//
// main.cpp for cpp_babel in /home/vicky/Bureau/EPITECH/cpp_babel/server/sources
// 
// Made by Victorien Fischer
// Login   <victorien.fischer@epitech.eu>
// 
// Started on  Mon Sep 25 06:33:17 2017 Victorien Fischer
// Last update Mon Sep 25 07:16:07 2017 Victorien Fischer
//

#include "Serveur/Server.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

/*
** getUsage
*/
static std::string	getUsage(std::string binary)
{
  std::stringstream		ss;

  ss << "Usage: ";
  ss << binary;
  ss << " [port]";
  return (ss.str());
}

/*
** IsValidPort
*/
int	isValidPort(unsigned short port)
{
  return (port > 1000);
}

/*
** Print Usage
*/
int	printUsage(std::string binary)
{
  std::cerr << getUsage(binary) << std::endl;
  return (1);
}

/*
** main
*/
int			main(int ac, char **av)
{
  unsigned short	port;

    if (ac == 2) {
        boost::asio::io_service io_service;

        port = std::atoi(av[1]);
        if (isValidPort(port)) {
            boost::asio::ip::tcp::endpoint  endpoint(boost::asio::ip::tcp::v4(), port);
            babel::Server   server(io_service, endpoint);
            io_service.run();
            return 0;
        }
    }
    return 1;
}
