/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:15:12 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 16:15:12 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_cleanup(t_shell *shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->arena)
		free_arena(&shell->arena);
	if (shell->exec->fd)
		free(shell->exec->fd);
	if (shell->exec)
		free(shell->exec);
}
