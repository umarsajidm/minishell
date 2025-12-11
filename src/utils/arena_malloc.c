#include "minishell.h"

/* Initialize a new arena block with given size
 * - Returns pointer to arena or NULL on allocation failure
 */
t_arena	*init_arena(size_t size)
{
	t_arena	*arena;

	if (!size)
		return (NULL);
	arena = malloc(sizeof(t_arena));
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

/* Create a new bigger arena block and link it
 * - Returns pointer to new arena or NULL on failure
 */
t_arena	*new_bigger_arena(t_arena *current, size_t size)
{
	size_t	new_size;
	t_arena	*new_arena;

	new_size = size * GROWTH_FACTOR;
	if (new_size < size)
		new_size = size;
	new_arena = init_arena(new_size);
	if (!new_arena)
		return (NULL);
	while (current->next)
		current = current->next;
	current->next = new_arena;
	return (new_arena);
}

/* Allocate memory from arena; grow arena if needed
 * - Returns pointer to allocated memory or NULL on failure
 */
void	*arena_alloc(t_arena **arena, size_t size)
{
	t_arena	*current;
	void	*ptr;
	size_t	alignment_size;

	if (!arena || !*arena || size == 0)
		return (NULL);

	// Align size to a multiple of sizeof(void*)
	alignment_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);

	current = *arena;
	// Find the last arena that might have space
	while (current->next)
		current = current->next;

	if (alignment_size > (current->buffer - current->offset))
	{
		current = new_bigger_arena(current, alignment_size);
		if (!current)
			return (NULL);
	}
	ptr = (char *)current->memory_block + current->offset;
	current->offset += alignment_size;
	return (ptr);
}

// void	*arena_alloc(t_arena **arena, size_t size)
// {
// 	t_arena	*head;
// 	t_arena	*current;
// 	void	*ptr;

// 	if (!arena || !*arena || size == 0)
// 		return (NULL);
// 	head = *arena;
// 	current = head;
// 	/* find last arena block */
// 	while (current->next)
// 		current = current->next;
// 	/* if not enough space in last block, grow by appending a new block */
// 	if (size > (current->buffer - current->offset))
// 	{
// 		current = new_bigger_arena(current, size);
// 		if (!current)
// 			return (NULL);
// 		/* DON'T update *arena — keep head unchanged */
// 	}
// 	ptr = (char *)current->memory_block + current->offset;
// 	current->offset += size;
// 	return (ptr);
// }


/* Re-allocate an arena-allocated block to a larger size
 * - Copies old_size bytes from old_ptr into newly allocated arena memory
 */
void	*arena_realloc(t_arena **arena, void *old_ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!arena || !*arena || new_size == 0)
		return (NULL);
	if (!old_ptr)
		return (arena_alloc(arena, new_size));
	new_ptr = arena_alloc(arena, new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, old_ptr, old_size);
	return (new_ptr);
}