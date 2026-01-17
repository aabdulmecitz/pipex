/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:25:00 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_from_node(t_node *node, char **envp, t_gc **gc)
{
	char	*cmd_path;

	if (!node || !node->args[0])
	{
		err(RED "Error: Command not found\n" RESET);
		exit(127);
	}
	cmd_path = find_valid_path(node->args[0], envp, gc);
	if (!cmd_path)
	{
		err(RED "Error: Command not found\n" RESET);
		exit(127);
	}
	execve(cmd_path, node->args, envp);
	err(RED "Error: execve failed\n" RESET);
	exit(1);
}

void	execute(char *argv, char **envp, t_gc **gc)
{
	t_node	*head;

	head = NULL;
	add_command(&head, argv, gc);
	if (head)
		execute_from_node(head, envp, gc);
}

int	is_here_doc(char *str)
{
	if (!str)
		return (0);
	return (ft_strncmp(str, "here_doc", 8) == 0);
}

void	wait_children(void)
{
	while (waitpid(-1, NULL, 0) != -1)
		;
}
