/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:28:15 by user42            #+#    #+#             */
/*   Updated: 2024/04/26 16:34:08 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <vector>
#include <string>
#include <poll.h>
#include <sstream>
#include <signal.h>

void                        addPollfd(std::vector<pollfd>& fds, int fd, short events);
std::vector<std::string>    split(std::string message);

#endif