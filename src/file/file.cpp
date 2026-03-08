/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:14:40 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/08 23:19:00 by zcadinot         ###   ########.fr       */
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

bool copy_file(const std::string& src, const std::string& dst)
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

bool create_user_file()
{
    const std::string base_dir =
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/user";

    const char* user_env = std::getenv("USER");
    if (!user_env)
        return false;

    std::string username(user_env);
    std::string filepath = base_dir + "/" + username;

    struct stat st;
    if (stat(base_dir.c_str(), &st) != 0)
    {
        if (mkdir(base_dir.c_str(), 0777) != 0)
            return false;
    }

    std::ofstream file(filepath);
    if (!file)
        return false;

    file.close();

    if (chmod(filepath.c_str(), 0777) != 0)
        return false;

    return true;
}

bool cp_bin_to_path(const std::string &binary_name)
{
    const std::string src =
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/ft_connect";

    if (binary_name.empty())
        return false;

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

        std::string dst = dirs[i] + "/" + binary_name;

        if (copy_file(src, dst))
            return true;
    }

    return false;
}
