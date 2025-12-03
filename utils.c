/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:20:22 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*make_path(char *uncompleted_path, char *cmd)
{
	char	*full_path;
	char	*temp;

	temp = ft_strjoin(uncompleted_path, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*find_valid_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	i = 0;
	path_var = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	i = 0;
	while (paths[i])
	{
		full_path = make_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL);
}

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
		ft_putstr_fd(RED "Error: Command not found\n" RESET, 2);
		exit(127);
	}
	cmd_path = find_valid_path(node->args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd(RED "Error: Command not found\n" RESET, 2);
		exit(127);
	}
	execve(cmd_path, node->args, envp);
	ft_putstr_fd(RED "Error: execve failed\n" RESET, 2);
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

void	parent_process(char **argv, char **envp, int *fd)
{
	int		outfile;
	int		status;
	pid_t	pid2;
	t_node	*head;

	head = NULL;
	add_command(&head, argv[3]);
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
			execute_from_node(head, envp);
	}
	if (pid2 == -1)
		err("fork");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	waitpid(pid2, &status, 0);
}
