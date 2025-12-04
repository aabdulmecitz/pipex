/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:36 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:25:00 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_gc	*gc;

	gc = NULL;
	if (argc < 5)
		err_args();
	if (is_here_doc(argv[1]))
		execute_here_doc(argv, argc, envp, &gc);
	else
		execute_multiple_pipes(argv, argc, envp, &gc);
	gc_free(gc);
	return (0);
}
