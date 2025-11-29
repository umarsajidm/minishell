#include "minishell.h"

/* Duplicate a C string into arena memory
 * - Returns pointer inside arena or NULL on failure
 */
char	*arena_strdup(t_arena **arena, const char *s)
{
	char	*dst;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dst = arena_alloc(arena, len + 1);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, s, len + 1);
	return (dst);
}


/* Free all arenas in the list
 * - Releases every buffer and arena struct
 */
void	free_arena(t_arena **arena)
{
	t_arena	*tmp;

	if (!arena || !*arena)
		return ;
	while (*arena)
	{
		tmp = (*arena)->next;
		free((*arena)->memory_block);
		free(*arena);
		*arena = tmp;
	}
}

/* Clear all arena memory chunks
 * - Resets offset to 0 for reuse
 * - Does not free memory, keeps arena structure
 */
void	arena_clear(t_arena **arena)
{
	t_arena	*cur;

	if (!arena || !*arena)
		return ;
	cur = *arena;
	while (cur)
	{
		cur->offset = 0;
		cur = cur->next;
	}
}
