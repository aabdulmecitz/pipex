/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:36 by aozkaya           #+#    #+#             */
/*   Updated: 2026/01/10 17:16:42 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_gc		*gc;
	t_pipe_data	data;

	gc = NULL;
	if (argc < 5)
	{
		if (argc >= 3 && is_here_doc(argv[1]))
			;
		else
			err_args();
	}
	data.argv = argv;
	data.argc = argc;
	data.envp = envp;
	data.gc = &gc;
	if (is_here_doc(argv[1]))
		execute_here_doc(data);
	else
		execute_multiple_pipes(data);
	gc_free(gc);
	return (0);
}
