/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:23 by aozkaya           #+#    #+#             */
/*   Updated: 2026/01/10 17:16:42 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	err(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	err_args(void)
{
	ft_putstr_fd(RED "Error: Invalid number of arguments.\n" RESET, 2);
	ft_putstr_fd(GREY "Usage: ./pipex_bonus <infile> <cmd1> <cmd2> <outfile>\n"
		RESET, 2);
	ft_putstr_fd(GREY "       ./pipex_bonus here_doc <LIMITER> <cmd1> <cmd2> <outfile>\n"
		RESET, 2);
	exit(EXIT_FAILURE);
}
