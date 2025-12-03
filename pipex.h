/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:29:43 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 19:36:09 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# define GREEN				"\033[0;32m"
# define RED 				"\033[1;31m"
# define GREY 				"\033[0;90m"
# define CYAN				"\033[1;96m"
# define RESET 				"\033[0m"

typedef struct s_pipex
{
    int		infile;
    int		outfile;
    int		pipe_fd[2];
    pid_t	pid1;
    pid_t	pid2;
}				t_pipex;

typedef struct s_cmd
{
    char	**cmd_args;
    char	*cmd_path;
}				t_cmd;


//error handling
void	err(char *msg);
void	err_args(void);
void	child_process(char **argv, char **envp, int *fd);
void	execute(char *argv, char **envp);
char	*find_valid_path(char *cmd, char **envp);
char	*make_path(char *uncompleted_path, char *cmd);

//garbage collector

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
}	t_gc;

void	*gc_malloc(t_gc **gc, size_t size);
void	gc_free(t_gc *gc);

#endif