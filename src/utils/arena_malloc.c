#include "minishell.h"

/* Initialize a new arena block with given size
 * - Returns pointer to arena or NULL on allocation failure
 */
t_arena	*init_arena(size_t size)
{
	t_arena	*arena;                             // new arena struct

	if (!size)
		return (NULL);                         // cannot create 0-sized arena
	arena = malloc(sizeof(t_arena));           // allocate arena struct
	if (!arena)
		return (NULL);                         // malloc failed
	arena->memory_block = malloc(size);        // allocate buffer
	if (!arena->memory_block)
		return (free(arena), NULL);            // buffer malloc failed
	arena->buffer = size;                      // set buffer size
	arena->offset = 0;                         // initial offset
	arena->next = NULL;                        // no next chunk
	return (arena);                            // return created arena
}

/* Create a new bigger arena block and link it
 * - Returns pointer to new arena or NULL on failure
 */
t_arena	*new_bigger_arena(t_arena *current, size_t size)
{
	size_t	new_size;                          // requested growth size
	t_arena	*new_arena;                        // new arena node

	new_size = size * GROWTH_FACTOR;           // grow (factor)
	if (new_size < size)
		new_size = size;                       // overflow safety
	new_arena = init_arena(new_size);          // create new arena
	if (!new_arena)
		return (NULL);                         // creation failed
	while (current->next)                      // find last arena
		current = current->next;               // move to next
	current->next = new_arena;                 // link new arena
	return (new_arena);                        // return new chunk
}

/* Allocate memory from arena; grow arena if needed
 * - Returns pointer to allocated memory or NULL on failure
 */
void	*arena_alloc(t_arena **arena, size_t size)
{
	t_arena	*current;                          // current arena block
	void	*ptr;                                // returned pointer

	if (!arena || !*arena || size == 0)
		return (NULL);                         // invalid input
	current = *arena;                          // start from head chunk
	if (size > (current->buffer - current->offset)) // not enough room
	{
		current = new_bigger_arena(current, size); // add bigger chunk
		if (!current)
			return (NULL);                     // failed to grow
		*arena = current;                      // update arena pointer
	}
	ptr = (char *)current->memory_block + current->offset; // compute ptr
	current->offset += size;                    // advance offset
	return (ptr);                              // return block
}

/* Re-allocate an arena-allocated block to a larger size
 * - Copies old_size bytes from old_ptr into newly allocated arena memory
 * - If old_ptr is NULL behaves like arena_alloc
 */
void	*arena_realloc(t_arena **arena, void *old_ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;                            // new allocated pointer

	if (!arena || !*arena || new_size == 0)
		return (NULL);                         // invalid input
	if (!old_ptr)
		return (arena_alloc(arena, new_size)); // act like alloc
	new_ptr = arena_alloc(arena, new_size);   // allocate new block in arena
	if (!new_ptr)
		return (NULL);                         // allocation failed
	ft_memcpy(new_ptr, old_ptr, old_size);    // copy previous content
	return (new_ptr);                         // return new ptr
}

/* Duplicate a C string into arena memory
 * - Returns pointer inside arena or NULL on failure
 */
char	*arena_strdup(t_arena **arena, const char *s)
{
	char	*dst;                                 // destination in arena
	size_t	len;                                  // string length

	if (!s)
		return (NULL);                           // nothing to duplicate
	len = ft_strlen(s);                          // calculate length
	dst = arena_alloc(arena, len + 1);           // allocate in arena
	if (!dst)
		return (NULL);                           // allocation failed
	ft_memcpy(dst, s, len + 1);                  // copy bytes including '\0'
	return (dst);                                // return duplicated string
}

/* Free all arenas in the list
 * - Releases every buffer and arena struct
 */
void	free_arena(t_arena **arena)
{
	t_arena	*tmp;                                 // temp pointer

	if (!arena || !*arena)
		return ;                                // nothing to free
	while (*arena)
	{
		tmp = (*arena)->next;                   // save next
		free((*arena)->memory_block);           // free buffer
		free(*arena);                           // free struct
		*arena = tmp;                           // advance
	}
}
