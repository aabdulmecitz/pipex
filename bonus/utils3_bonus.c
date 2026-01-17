/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:25:00 by aozkaya           #+#    #+#             */
/*   Updated: 2026/01/17 04:40:00 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_process(char *cmd, int in_fd, int out_fd, t_pipe_data data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		err("fork");
	if (pid == 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			err("dup2 in");
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			err("dup2 out");
		close(in_fd);
		close(out_fd);
		execute(cmd, data.envp, data.gc);
		exit(1);
	}
}

void	execute_multiple_pipes(t_pipe_data data)
{
	int	infile;
	int	outfile;
	int	fd[2];
	int	prev_fd;
	int	i;

	infile = open(data.argv[1], O_RDONLY);
	if (infile == -1)
		err(data.argv[1]);
	outfile = open(data.argv[data.argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err(data.argv[data.argc - 1]);
	prev_fd = infile;
	i = 2;
	while (i < data.argc - 2)
	{
		if (pipe(fd) == -1)
			err("pipe");
		child_process(data.argv[i], prev_fd, fd[1], data);
		(close(prev_fd), close(fd[1]));
		prev_fd = fd[0];
		i++;
	}
	child_process(data.argv[i], prev_fd, outfile, data);
	(close(prev_fd), close(outfile));
}

void	execute_here_doc_write(int tmp_fd, char *limiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(limiter) + 1
			&& ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, tmp_fd);
		free(line);
	}
}

void	execute_here_doc(t_pipe_data data)
{
	int	tmp_fd;
	int	outfile;
	int	prev_fd;
	int	fd[2];
	int	i;

	outfile = open(data.argv[data.argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile == -1)
		err(data.argv[data.argc - 1]);
	tmp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		err("open tmp");
	execute_here_doc_write(tmp_fd, data.argv[2]);
	close(tmp_fd);
	prev_fd = open(".here_doc_tmp", O_RDONLY);
	if (prev_fd == -1)
		err(".here_doc_tmp");
	unlink(".here_doc_tmp");
	i = 3;
	while (i < data.argc - 2)
	{
		if (pipe(fd) == -1)
			err("pipe");
		child_process(data.argv[i], prev_fd, fd[1], data);
		(close(prev_fd), close(fd[1]));
		prev_fd = fd[0];
		i++;
	}
	child_process(data.argv[i], prev_fd, outfile, data);
	(close(prev_fd), close(outfile));
}




