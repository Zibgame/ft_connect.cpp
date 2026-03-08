/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_pr.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 23:25:28 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/08 23:27:51 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>

static void rewrite_zshrc(const std::string &path)
{
    std::ifstream file;
    std::vector<std::string> lines;
    std::string line;

    file.open(path);
    if (!file)
        return;

    while (std::getline(file, line))
    {
        if (line.find("export PATH=\"$PATH:$HOME/.local/bin\"") != std::string::npos)
            continue;
        if (line.find("disable true") != std::string::npos)
            continue;
        lines.push_back(line);
    }
    file.close();

    std::ofstream out(path);
    if (!out)
        return;

    size_t i = 0;
    while (i < lines.size())
    {
        out << lines[i] << "\n";
        i++;
    }
    out.close();
}

void remove_persistence()
{
    std::string zshrc;
    std::string bin;

    zshrc = get_zshrc_path();
    rewrite_zshrc(zshrc);

    bin = "/home/";
    bin += get_current_user();
    bin += "/.local/bin/true";

    std::remove(bin.c_str());
}
