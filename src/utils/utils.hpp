/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:28:15 by user42            #+#    #+#             */
/*   Updated: 2024/03/12 09:32:36 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <vector>
#include <string>
#include <poll.h>
#include <sstream>

void                        addPollfd(std::vector<pollfd>& fds, int fd, short events);
std::vector<std::string>    split(std::string message);

#endif