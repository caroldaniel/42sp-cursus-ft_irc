/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:02:55 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 14:08:55 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

int main(int argc, char **argv)
{
    int port;

    // Get arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    port = std::atoi(argv[1]);

    // Start server
    Server server(port);
    if (!server.start()) {
        std::cerr << "Error starting server" << std::endl;
        return 1;
    }
    
    return 0;
}
