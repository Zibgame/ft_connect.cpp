/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:14:40 by zcadinot          #+#    #+#             */
/*   Updated: 2026/02/26 13:15:55 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

static std::vector<std::string> split_path(const std::string& path)
{
    std::vector<std::string> dirs;
    size_t start = 0;
    size_t end;

    while ((end = path.find(':', start)) != std::string::npos)
    {
        dirs.push_back(path.substr(start, end - start));
        start = end + 1;
    }
    dirs.push_back(path.substr(start));
    return dirs;
}

static bool ensure_directory(const std::string& dir)
{
    struct stat st;

    if (stat(dir.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);

    if (mkdir(dir.c_str(), 0755) != 0)
        return false;

    return true;
}

static bool copy_file(const std::string& src, const std::string& dst)
{
    std::ifstream in(src, std::ios::binary);
    if (!in)
        return false;

    std::ofstream out(dst, std::ios::binary);
    if (!out)
        return false;

    out << in.rdbuf();

    in.close();
    out.close();

    if (chmod(dst.c_str(), 0755) != 0)
        return false;

    return true;
}

bool cp_bin_to_path()
{
    const std::string src =
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/ft_connect";

    const char* path_env = std::getenv("PATH");
    if (!path_env)
        return false;

    std::vector<std::string> dirs = split_path(path_env);

    for (size_t i = 0; i < dirs.size(); ++i)
    {
        if (!ensure_directory(dirs[i]))
            continue;

        if (access(dirs[i].c_str(), W_OK) != 0)
            continue;

        std::string dst = dirs[i] + "/ft_connect";

        if (copy_file(src, dst))
            return true;
    }

    return false;
}