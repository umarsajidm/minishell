/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:02:20 by musajid           #+#    #+#             */
/*   Updated: 2025/10/28 00:02:20 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_arena {
	void	*memory_block; // start of memory/pointer to the start of memory block
	size_t	buffer;	// total size of memory
	size_t	offset; //position from the starting point of buffer
	t_arena	*next;
}               t_arena;

t_arena *init_arena(size_t size)
{
	if (!size)
		return (NULL);
	t_arena *arena = ft_calloc(1, sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->memory_block = malloc(size);
	if (!arena->memory_block)
		return (free(arena), NULL);
	arena->buffer = size;		
}

void	*allocation(t_arena *arena, size_t size)
{
	if (!arena || !size)
		return (NULL);
	if (!arena->memory_block)
		
	if (size > (arena->buffer - arena->offset))

}
