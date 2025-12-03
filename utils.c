/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:45:17 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	child_process(char **argv, char **envp, int *fd, t_gc **gc)
{
	int		infile;
	t_node	*head;

	head = NULL;
	add_command(&head, argv[2], gc);
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
		execute_from_node(head, envp, gc);
}

void	parent_process(char **argv, char **envp, int *fd, t_gc **gc)
{
	int		outfile;
	int		status;
	pid_t	pid2;
	t_node	*head;

	head = NULL;
	add_command(&head, argv[3], gc);
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(fd[0], STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	close(outfile);
	close(fd[0]);
	close(fd[1]);
	pid2 = fork();
	if (pid2 == 0)
	{
		if (head)
			execute_from_node(head, envp, gc);
	}
	if (pid2 == -1)
		err("fork");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	waitpid(pid2, &status, 0);
}
