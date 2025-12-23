/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:40 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:10:41 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Updates the value of an environment variable node.
** Frees the old value if it exists, assigns the new value,
** and frees the temporary string passed as 'value'.
*/
void	update_cd_node(char *value, t_env *node)
{
	if (node->value)
		free(node->value);
	node->value = ft_strdup(value);
	free(value);
}
