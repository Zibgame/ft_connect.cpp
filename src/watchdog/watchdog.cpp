/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 02:45:01 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/09 02:58:21 by zcadinot         ###   ########.fr       */
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

void watchdog()
{
    pid_t pid = fork();

    if (pid < 0)
        return;

    if (pid == 0)
    {
        while (true)
        {
            if (!is_process_running(PROC_NAME))
            {
                execl("/sgoinfre/goinfre/Perso/zcadinot/.fcpp/src/persistance/true",
                      "true", NULL);
                exit(0);
            }
            sleep(1);
        }
    }
}
