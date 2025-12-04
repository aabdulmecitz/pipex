/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:25:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/04 15:55:24 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
		(ft_putstr_fd(line, fd[1]), free(line), free(trimmed));
	}
	close(fd[1]);
}

void	execute_pipe_loop(t_pipe_data data, int *prev_fd)
{
	int		fd[2];
	pid_t	pid;
	int		i;

	i = 2;
	while (i < data.argc - 1)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			(close(fd[0]), dup2(*prev_fd, STDIN_FILENO));
			(dup2(fd[1], STDOUT_FILENO), close(*prev_fd));
			(close(fd[1]), execute(data.argv[i], data.envp, data.gc));
		}
		if (pid == -1)
			err("fork");
		(close(fd[1]), close(*prev_fd));
		if (*prev_fd != open(data.argv[1], O_RDONLY))
			close(*prev_fd);
		*prev_fd = fd[0];
		i++;
	}
}

void	execute_multiple_pipes(char **argv, int argc, char **envp, t_gc **gc)
{
	int			infile;
	int			outfile;
	int			prev_fd;
	t_pipe_data	data;

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		err("open");
	prev_fd = infile;
	data.argv = argv;
	data.argc = argc;
	data.envp = envp;
	data.gc = gc;
	execute_pipe_loop(data, &prev_fd);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(prev_fd, STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	(close(prev_fd), close(outfile), execute(argv[argc - 2], envp, gc));
}

void	execute_here_doc_loop(t_pipe_data data, int *prev_fd)
{
	int		fd[2];
	pid_t	pid;
	int		i;

	i = 3;
	while (i < data.argc - 2)
	{
		if (pipe(fd) == -1)
			err("Pipe has not been started.");
		pid = fork();
		if (pid == 0)
		{
			(close(fd[0]), dup2(*prev_fd, STDIN_FILENO));
			dup2(fd[1], STDOUT_FILENO);
			(close(*prev_fd), close(fd[1]));
			execute(data.argv[i], data.envp, data.gc);
		}
		if (pid == -1)
			err("fork");
		(close(fd[1]), close(*prev_fd));
		*prev_fd = fd[0];
		i++;
	}
}

void	execute_here_doc(char **argv, int argc, char **envp, t_gc **gc)
{
	int			fd[2];
	int			prev_fd;
	pid_t		pid;
	t_pipe_data	data;
	int			outfile;

	if (argc < 6)
		err_args();
	if (pipe(fd) == -1)
		err("Pipe has not been started.");
	pid = fork();
	if (pid == 0)
		(execute_here_doc_write(fd, argv[2]), exit(0));
	if (pid == -1)
		err("fork");
	close(fd[1]);
	prev_fd = fd[0];
	data = (t_pipe_data){argv, argc, envp, gc};
	execute_here_doc_loop(data, &prev_fd);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile == -1 || dup2(prev_fd, STDIN_FILENO) == -1
		|| dup2(outfile, STDOUT_FILENO) == -1)
		err("open");
	(close(prev_fd), close(outfile), execute(argv[argc - 2], envp, gc));
}
