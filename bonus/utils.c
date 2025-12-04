/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:24:23 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	return (ft_strncmp(str, "here_doc", 8) == 0);
}

void	execute_multiple_pipes(char **argv, int argc, char **envp, t_gc **gc)
{
	int		infile;
	int		outfile;
	int		fd[2];
	pid_t	pid;
	int		i;

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		err("open");
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			if (i == 2)
			{
				if (dup2(infile, STDIN_FILENO) == -1)
					err("dup2");
			}
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				err("dup2");
			close(fd[1]);
			execute(argv[i], envp, gc);
		}
		if (pid == -1)
			err("fork");
		close(fd[1]);
		if (i != 2)
			close(infile);
		infile = fd[0];
		i++;
	}
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(infile, STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	close(infile);
	close(outfile);
	execute(argv[argc - 2], envp, gc);
}

void	execute_here_doc(char **argv, int argc, char **envp, t_gc **gc)
{
	int		fd[2];
	pid_t	pid;
	int		i;
	int		outfile;
	char	*line;

	if (argc < 6)
		err_args();
	if (pipe(fd) == -1)
		err("Pipe has not been started.");
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			ft_putstr_fd("> ", 1);
			line = get_next_line(0);
			if (!line)
				break ;
			if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
			{
				free(line);
				break ;
			}
			ft_putstr_fd(line, fd[1]);
			free(line);
		}
		close(fd[1]);
		exit(0);
	}
	if (pid == -1)
		err("fork");
	close(fd[1]);
	i = 3;
	while (i < argc - 2)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			execute(argv[i], envp, gc);
		}
		if (pid == -1)
			err("fork");
		close(fd[1]);
		i++;
	}
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(fd[0], STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	close(fd[0]);
	close(outfile);
	execute(argv[argc - 2], envp, gc);
}
