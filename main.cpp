/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayache- <mayache-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:57:00 by mayache-          #+#    #+#             */
/*   Updated: 2024/07/26 09:53:10 by mayache-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        cerr << "Must be run as follows: : ./ircserv <port> <password>" << endl;
        return 1;
    }
    try
    {
        string  port = av[1];
        string password = av[2];
        Server srv;
        srv.start(av[1], av[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}