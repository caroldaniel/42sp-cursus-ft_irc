/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:02:55 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/08 11:42:05 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

int main(int argc, char **argv)
{
    // Get arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <server_password>" << std::endl;
        return 1;
    }
    std::string port = argv[1];
    // Check if port is a number
    if (port.find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Error: Port must be a number" << std::endl;
        return 1;
    }
    std::string password = argv[2];

    // Create and start server
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
