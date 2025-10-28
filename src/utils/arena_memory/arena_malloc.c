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

void	*allocation(t_arena **arena, size_t size); //prototype of arena malloc



typedef struct s_arena {
	void	*memory_block; // start of memory/pointer to the start of memory block
	size_t	buffer;	// total size of memory_block
	size_t	offset; //position from the starting point of memory_block
	t_arena	*next;
}               t_arena;

#define GROWTH_FACTOR 2

t_arena *init_arena(size_t size)
{
	if (!size)
		return (NULL);
	t_arena *arena = malloc(sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->memory_block = malloc(size);
	if (!arena->memory_block)
		return (free(arena), NULL);
	arena->buffer = size;
	arena->offset = 0;
	arena->next = NULL;
	return (arena);
}

t_arena	*new_bigger_arena(t_arena *current_arena, size_t size)
{
	size_t	new_size;
	t_arena	*new_bigge_arena;
	
	new_size = size * GROWTH_FACTOR;
	if (new_size < size)
		new_size = size;
	new_bigge_arena = init_arena(new_size);
	if (!new_bigger_arena)
		return (NULL);
	while (current_arena->next)
		current_arena = current_arena->next;
	current_arena->next = new_bigge_arena;
	return (new_bigger_arena);
	
}

void	*allocation(t_arena **arena, size_t size)
{
	t_arena	*temp;
	void	*where_to_start_in_memory_block;

	temp = *arena;
	if (!arena || !size)
		return (NULL);
	if (size > (temp->buffer - temp->offset))
	{
		temp = new_bigger_arena(temp, size);
		if (!temp)
			return (NULL);
		*arena = temp;
	}
	where_to_start_in_memory_block = (char *)temp->memory_block + temp->offset;
	temp->offset += size; //it updates the offset(position), so that we know where to start next time.
	return (where_to_start_in_memory_block);
}
void	free_arena(t_arena **arena)
{
	t_arena	*temp;

	while (*arena)
	{
		temp = (*arena)->next;
		free((*arena)->memory_block);
		free(*arena);
		*arena = temp;
	}
}
