/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:28:15 by user42            #+#    #+#             */
/*   Updated: 2024/03/11 20:04:06 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <vector>
#include <string>
#include <poll.h>
#include <sstream>

// Function to add a pollfd structure to a vector
void addPollfd(std::vector<pollfd>& fds, int fd, short events);

// Function to split a string into a vector of strings using whitespace as delimiter
std::vector<std::string> split(std::string message);

#endif