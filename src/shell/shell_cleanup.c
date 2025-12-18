/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:10:09 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 16:10:09 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_cleanup(t_shell *shell)
{
	rl_clear_history();
	free_env(shell->env);
	free_arena(&shell->arena);
	if (shell->exec && shell->exec->fd)
	{
		close_fd(shell->exec->fd);
		free(shell->exec->fd);
		free(shell->exec);
	}
}
