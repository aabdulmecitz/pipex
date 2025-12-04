/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/04 16:46:53 by aozkaya          ###   ########.fr       */
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

void	child_process(char **argv, char **envp, int *fd, t_gc **gc)
{
	int		infile;

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
	execute(argv[2], envp, gc);
}

void	parent_process(char **argv, char **envp, int *fd, t_gc **gc)
{
	int		outfile;
	int		status;
	pid_t	pid2;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		err("open");
	if (dup2(fd[0], STDIN_FILENO) == -1)
		err("dup2");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		err("dup2");
	(close(outfile), close(fd[0]), close(fd[1]));
	pid2 = fork();
	if (pid2 == 0)
		execute(argv[3], envp, gc);
	if (pid2 == -1)
		err("fork");
	(close(STDIN_FILENO), close(STDOUT_FILENO));
	waitpid(pid2, &status, 0);
	gc_free(*gc);
	exit(0);
}
