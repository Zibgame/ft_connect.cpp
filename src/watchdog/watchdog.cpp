/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 02:45:01 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/09 03:05:09 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"

bool is_process_running(const std::string &name)
{
    std::string cmd = "pgrep " + name;
    FILE *pipe = popen(cmd.c_str(), "r");
    char buffer[32];

    if (!pipe)
        return false;

    bool found = fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);
    return found;
}

pid_t watchdog()
{
    pid_t pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0)
    {
        while (true)
        {
            if (!is_process_running(PROC_NAME))
            {
                execl("/path/ft_connect", "ft_connect", NULL);
                exit(1);
            }
            sleep(1);
        }
    }
    return pid;
}

void save_watchdog_pid(pid_t pid)
{
    std::ofstream file("/tmp/.ft_watchdog_pid");

    if (!file)
        return;

    file << pid;
}

pid_t get_watchdog_pid()
{
    std::ifstream file("/tmp/.ft_watchdog_pid");
    pid_t pid;

    if (!file)
        return -1;

    file >> pid;
    return pid;
}
