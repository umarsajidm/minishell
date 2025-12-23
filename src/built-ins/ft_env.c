/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:30 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:16:46 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Executes the 'env' built-in command.
** Iterates through the linked list of environment variables.
** Prints key=value pairs for variables that have a non-NULL value.
** Skips variables without a value.
** Returns 0.
*/
int	ft_env(t_env *head)
{
	while (head)
	{
		if (!head->value)
		{
			head = head->next;
			continue ;
		}
		ft_putstr_fd(head->key, STDOUT_FILENO);
		if (head->value)
		{
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(head->value, STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		head = head->next;
	}
	return (0);
}
