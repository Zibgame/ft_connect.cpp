/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:27:21 by zcadinot          #+#    #+#             */
/*   Updated: 2026/02/25 16:29:07 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"

std::string get_first_line(const std::string &path)
{
    std::ifstream file(path.c_str());
    std::string line;

    if (!file)
        return "";

    std::getline(file, line);
    return line;
}

void kill_old_instances()
{
    pid_t current_pid = getpid();
    std::stringstream ss;

    ss << "pgrep " << PROC_NAME;

    FILE *pipe = popen(ss.str().c_str(), "r");
    if (!pipe)
        return;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe))
    {
        pid_t pid = atoi(buffer);
        if (pid != current_pid)
            kill(pid, SIGKILL);
    }

    pclose(pipe);
}

void clear_file(const std::string &path)
{
    std::ofstream file(path.c_str(), std::ios::trunc);

    if (!file)
    {
        /* std::cout << "Erreur ouverture\n"; */
        return;
    }
}

data create_cmd_struct(const std::string &line)
{
    data cmd;
    size_t pos;

    pos = line.find(';');
    if (pos == std::string::npos)
    {
        /* std::cout << "Format invalide\n"; */
        return cmd;
    }

    cmd.name = line.substr(0, pos);
    cmd.command = line.substr(pos + 1);

    return cmd;
}

void exec_cmd(const std::string &cmd, const bool headless)
{
    if (cmd.empty())
    {
        /* std::cout << "Commande vide\n"; */
        return;
    }

    if (headless)
    {
        std::string silent_cmd = cmd + " > /dev/null 2>&1";
        system(silent_cmd.c_str());
    }
    else
    {
        system(cmd.c_str());
    }
}

static void daemonize(void)
{
    pid_t pid = fork();

    if (pid < 0)
        exit(1);

    if (pid > 0)
        exit(0);
    if (pid == 0)
        prctl(PR_SET_NAME, PROC_NAME, 0, 0, 0);

    setsid();
}

int main(void)
{
    std::string content;
    data cmd;

    kill_old_instances();
    prctl(PR_SET_NAME, PROC_NAME, 0, 0, 0);
    daemonize();
    while (true)
    {
        content = get_first_line(CMD_FILE);
        if (!content.length())
            continue;
        cmd = create_cmd_struct(content);
        if (!cmd.name.length() || !cmd.command.length())
            continue;
        exec_cmd(cmd.command, 0);
        clear_file(CMD_FILE);
        usleep(LOOP_DELAY);
    }
    return 0;
}
