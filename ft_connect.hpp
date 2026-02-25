/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_connect.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:28:44 by zcadinot          #+#    #+#             */
/*   Updated: 2026/02/25 16:11:47 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/prctl.h>

#define CMD_FILE "command/command"
#define PROC_NAME "ft_connect"
#define LOOP_DELAY 500000 /* En Us */

struct data
{
    std::string name;
    std::string command;
};
