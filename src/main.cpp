/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:02:55 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 19:18:28 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

int main(int argc, char **argv)
{
    int port;
    std::string password;

    // Get arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <server_password>" << std::endl;
        return 1;
    }
    port = std::atoi(argv[1]);
    password = argv[2];

    // Start server
    Server ircserv(port, password);
    try {
        ircserv.create_socket();
        ircserv.start();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
