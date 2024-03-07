/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:28:58 by user42            #+#    #+#             */
/*   Updated: 2024/03/06 21:30:14 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void addPollfd(std::vector<pollfd>& fds, int fd, short events) {
    pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}
