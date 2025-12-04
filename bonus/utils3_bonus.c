/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:25:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:36:54 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_pipe_child(int fd[2], char *cmd, char **envp, t_gc **gc)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execute(cmd, envp, gc);
}

void	execute_here_doc_write(int fd[2], char *limiter)
{
	char	*line;
	char	*trimmed;
	size_t	len;

	close(fd[0]);
	len = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		trimmed = ft_strtrim(line, "\n");
		if (ft_strncmp(trimmed, limiter, len) == 0 && trimmed[len] == '\0')
		{
			free(line);
			free(trimmed);
			break ;
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
		free(trimmed);
	}
	close(fd[1]);
}

void	execute_multiple_pipes(char **argv, int argc, char **envp, t_gc **gc)
{
	int		infile;
	int		outfile;
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		i;

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		err("open");
	prev_fd = infile;
	i = 2;
	while (i < argc - 1)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(prev_fd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(prev_fd);
			close(fd[1]);
			execute(argv[i], envp, gc);
		}
		if (pid == -1)
			err("fork");
		close(fd[1]);
		if (prev_fd != infile)
			close(prev_fd);
		prev_fd = fd[0];
		i++;
	}
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(prev_fd, STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	close(prev_fd);
	close(outfile);
	execute(argv[argc - 2], envp, gc);
}

void	execute_here_doc(char **argv, int argc, char **envp, t_gc **gc)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		i;
	int		outfile;

	if (argc < 6)
		err_args();
	if (pipe(fd) == -1)
		err("Pipe has not been started.");
	pid = fork();
	if (pid == 0)
	{
		execute_here_doc_write(fd, argv[2]);
		exit(0);
	}
	if (pid == -1)
		err("fork");
	close(fd[1]);
	prev_fd = fd[0];
	i = 3;
	while (i < argc - 2)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(prev_fd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(prev_fd);
			close(fd[1]);
			execute(argv[i], envp, gc);
		}
		if (pid == -1)
			err("fork");
		close(fd[1]);
		close(prev_fd);
		prev_fd = fd[0];
		i++;
	}
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(prev_fd, STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	close(prev_fd);
	close(outfile);
	execute(argv[argc - 2], envp, gc);
}
