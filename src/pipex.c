/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:36 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:58:41 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;
	t_gc	*gc;

	gc = NULL;
	if (argc != 5)
		err_args();
	if (pipe(fd) == -1)
		err("Pipe has not been started.");
	pid = fork();
	if (pid == 0)
		child_process(argv, envp, fd, &gc);
	if (pid == -1)
		err("Fork Error!");
	if (pid != 0)
		parent_process(argv, envp, fd, &gc);
	gc_free(gc);
	return (0);
}
