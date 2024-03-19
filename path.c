/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-m <carlos-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:06:01 by carlos-m          #+#    #+#             */
/*   Updated: 2024/02/28 13:06:03 by carlos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*path_access(char **possible_paths, char **full_command, char *comand)
{
	char	*path;
	int		i;

	i = 1;
	path = ft_strjoin(ft_strjoin(possible_paths[0], "/"), full_command[0]);
	while (access(path, X_OK) != 0 && path != NULL)
	{
		free(path);
		if (possible_paths[i])
		{
			path = ft_strjoin(ft_strjoin(possible_paths[i], "/"), \
			full_command[0]);
			i++;
		}
		else
			return (ft_strdup(comand));
	}
	return (path);
}

char	*find_path(char *envp[], char *comand)
{
	int		i;
	char	**full_command;
	char	*path;
	char	**possible_paths;

	i = 0;
	while (envp[i] && ft_strncmp("PATH", envp[i], 4) != 0)
		i++;
	if (envp[i] == NULL)
		return (comand);
	full_command = ft_split(comand, ' ');
	path = envp[i] + 5;
	possible_paths = ft_split(path, ':');
	path = path_access(possible_paths, full_command, comand);
	return (path);
}
