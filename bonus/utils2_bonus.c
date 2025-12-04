/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:47:33 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:36:54 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*make_path(char *uncompleted_path, char *cmd, t_gc **gc)
{
	char	*full_path;
	char	*temp;

	temp = ft_strjoin(uncompleted_path, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	gc_add(gc, full_path);
	return (full_path);
}

char	*get_path_var(char **envp)
{
	int	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_valid_path(char *cmd, char **envp, t_gc **gc)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	if (cmd[0] == '/' && access(cmd, X_OK) == 0)
		return (cmd);
	path_var = get_path_var(envp);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	gc_add(gc, (void *)paths);
	i = 0;
	while (paths[i])
	{
		full_path = make_path(paths[i], cmd, gc);
		if (access(full_path, X_OK) == 0)
		{
			return (full_path);
		}
		i++;
	}
	return (NULL);
}

t_node	*create_node(char *cmd_str, t_gc **gc)
{
	t_node	*node;
	char	**args;
	int		i;

	node = (t_node *)gc_malloc(gc, sizeof(t_node));
	if (!node)
		return (NULL);
	node->data = ft_strdup(cmd_str);
	gc_add(gc, node->data);
	node->next = NULL;
	args = ft_split(cmd_str, ' ');
	if (!args)
		return (NULL);
	gc_add(gc, (void *)args);
	i = 0;
	while (args[i] && i < 999)
	{
		node->args[i] = args[i];
		i++;
	}
	node->args[i] = NULL;
	return (node);
}

t_node	*add_command(t_node **head, char *cmd_str, t_gc **gc)
{
	t_node	*new_node;
	t_node	*current;

	new_node = create_node(cmd_str, gc);
	if (!new_node)
		return (NULL);
	if (*head == NULL)
	{
		*head = new_node;
		return (new_node);
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	return (new_node);
}
