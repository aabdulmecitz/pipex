/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:28 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:40:50 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	gc_add_node(t_gc *gc, void *ptr)
{
	t_gc_node	*new_node;

	new_node = (t_gc_node *)malloc(sizeof(t_gc_node));
	if (!new_node)
		return (-1);
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;
	return (0);
}

void	gc_add(t_gc **gc, void *ptr)
{
	if (!gc || !ptr)
		return ;
	if (!*gc)
	{
		*gc = (t_gc *)malloc(sizeof(t_gc));
		if (!*gc)
			return ;
		(*gc)->head = NULL;
	}
	gc_add_node(*gc, ptr);
}

void	*gc_malloc(t_gc **gc, size_t size)
{
	void	*ptr;

	if (!gc)
		return (NULL);
	if (!*gc)
	{
		*gc = (t_gc *)malloc(sizeof(t_gc));
		if (!*gc)
			return (NULL);
		(*gc)->head = NULL;
	}
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (gc_add_node(*gc, ptr) == -1)
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}

void	gc_free(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	if (!gc)
		return ;
	current = gc->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	free(gc);
}
