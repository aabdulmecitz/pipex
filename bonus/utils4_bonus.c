/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 17:52:49 by aozkaya           #+#    #+#             */
/*   Updated: 2026/01/17 04:28:36 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_missing_args(t_pipe_data data, int fd_read)
{
	int		fd[2];
	int		i;
	pid_t	pid;

	i = 2;
	while (++i < data.argc - 1)
	{
		if (pipe(fd) == -1)
			err("pipe");
		pid = fork();
		if (pid == -1)
			err("fork");
		if (pid == 0)
		{
			(close(fd[0]), dup2(fd_read, STDIN_FILENO));
			(dup2(fd[1], STDOUT_FILENO), close(fd_read));
			(close(fd[1]), execute(data.argv[i], data.envp, data.gc));
		}
		(close(fd[1]), close(fd_read));
		fd_read = fd[0];
	}
	(dup2(fd_read, STDIN_FILENO), close(fd_read));
	execute(data.argv[data.argc - 1], data.envp, data.gc);
}

void	write_to_fd(int fd_read, int fd_write)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd_read);
		if (!line)
			break ;
		ft_putstr_fd(line, fd_write);
		free(line);
	}
}

void	handle_file_output(t_pipe_data data, int fd_read)
{
	int	outfile;

	if (data.argc == 4 && !find_valid_path(data.argv[3], data.envp, data.gc))
	{
		outfile = open(data.argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile == -1)
			err("open");
		write_to_fd(fd_read, outfile);
		close(outfile);
		close(fd_read);
		exit(0);
	}
}

void	execute_simple_heredoc(t_pipe_data data)
{
	int		fd_read;

	fd_read = open(".here_doc_tmp", O_RDONLY);
	if (fd_read == -1)
	{
		unlink(".here_doc_tmp");
		err("open");
	}
	unlink(".here_doc_tmp");
	if (data.argc == 3)
	{
		write_to_fd(fd_read, 1);
		close(fd_read);
		exit(0);
	}
	handle_file_output(data, fd_read);
	handle_missing_args(data, fd_read);
}
