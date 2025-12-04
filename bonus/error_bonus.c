/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:23 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 21:02:32 by aozkaya          ###   ########.fr       */
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
	ft_putstr_fd(GREY "Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>\n"
		RESET, 2);
	exit(EXIT_FAILURE);
}
