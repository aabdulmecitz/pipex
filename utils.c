/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aozkaya <aozkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:00 by aozkaya           #+#    #+#             */
/*   Updated: 2025/12/03 20:01:59 by aozkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*make_path(char *uncompleted_path, char *cmd)
{
    char	*full_path;
    char	*temp;

    temp = ft_strjoin(uncompleted_path, "/");
    full_path = ft_strjoin(temp, cmd);
    free(temp);
    return (full_path);
}

char	*find_valid_path(char *cmd, char **envp)
{
    char	**paths;
    char	*path_var;
    char	*full_path;
    int		i;

    i = 0;
    path_var = NULL;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_var = envp[i] + 5;
            break ;
        }
        i++;
    }
    if (!path_var)
        return (NULL);
    paths = ft_split(path_var, ':');
    i = 0;
    while (paths[i])
    {
        full_path = make_path(paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    ft_free_split(paths);
    return (NULL);
}

char    *add_node(t_node **head, char *data)
{
    t_node  *new_node;
    t_node  *temp;

    new_node = (t_node *)malloc(sizeof(t_node));
    if (!new_node)
        return (NULL);
    new_node->data = data;
    new_node->next = NULL;
    if (*head == NULL)
    {
        *head = new_node;
        return (data);
    }
    temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new_node;
    return (data);
}