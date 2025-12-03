/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:25:46 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:37:14 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


t_node	*create_node(char *cmd_str)
{
	t_node	*node;
	char	**args;
	int		i;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->data = ft_strdup(cmd_str);
	node->next = NULL;
	args = ft_split(cmd_str, ' ');
	if (!args)
		return (NULL);
	i = 0;
	while (args[i] && i < 999)
	{
		node->args[i] = args[i];
		i++;
	}
	node->args[i] = NULL;
	return (node);
}

t_node	*add_command(t_node **head, char *cmd_str)
{
	t_node	*new_node;
	t_node	*current;

	new_node = create_node(cmd_str);
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

void	execute_from_node(t_node *node, char **envp)
{
	char	*cmd_path;

	if (!node || !node->args[0])
	{
		err(RED "Error: Command not found\n" RESET);
		exit(127);
	}
	cmd_path = find_valid_path(node->args[0], envp);
	if (!cmd_path)
	{
		err(RED "Error: Command not found\n" RESET);
		exit(127);
	}
	execve(cmd_path, node->args, envp);
	err(RED "Error: execve failed\n" RESET);
	free(cmd_path);
	exit(1);
}

void	execute(char *argv, char **envp)
{
	t_node	*head;

	head = NULL;
	add_command(&head, argv);
	if (head)
		execute_from_node(head, envp);
}

void	child_process(char **argv, char **envp, int *fd)
{
	int		infile;
	t_node	*head;

	head = NULL;
	add_command(&head, argv[2]);
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		err("open");
	if (dup2(infile, STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		err("dup2");
	close(infile);
	close(fd[0]);
	close(fd[1]);
	if (head)
		execute_from_node(head, envp);
}
