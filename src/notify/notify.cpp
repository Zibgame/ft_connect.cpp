/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notify.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:16:50 by zcadinot          #+#    #+#             */
/*   Updated: 2026/02/26 14:22:12 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_connect.hpp"

bool notify_send(const std::string &message)
{
    std::string cmd = "notify-send \"" + message + "\"";
    system(cmd.c_str());
    return (true);
}

void notify_last_sender_warning(void)
{
    std::string sender = get_last_sender();
    std::string message;

    if (sender.empty())
        return;

    message = "⚠️ Warning ⚠️ ";
    message += sender;
    message += " a voulu vous envoyer une commande 💻";

    notify_send(message);
}