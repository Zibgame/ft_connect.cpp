/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:28:50 by zcadinot          #+#    #+#             */
/*   Updated: 2026/02/26 14:29:05 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"
#include <fstream>
#include <ctime>

std::string get_last_sender(void)
{
    std::ifstream file("/sgoinfre/goinfre/Perso/zcadinot/.fcpp/other/logs/logs");
    std::string line;
    std::string last_line;
    size_t pos;

    if (!file)
        return "";

    while (std::getline(file, line))
    {
        if (!line.empty())
            last_line = line;
    }

    if (last_line.empty())
        return "";

    pos = last_line.find(' ');
    if (pos == std::string::npos)
        return "";

    return last_line.substr(0, pos);
}