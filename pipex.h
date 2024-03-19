/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-m <carlos-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 11:19:58 by carlos-m          #+#    #+#             */
/*   Updated: 2024/01/21 11:19:59 by carlos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "Libft/libft.h"

typedef struct s_pipex
{
	char	**env;
	char	**argv;
	int		argc;
	int		**pipes;
	int		*childs;
	int		tam;

}	t_pipex;

char	*find_path(char *envp[], char *comand);
char	*path_access(char **possible_paths, char **full_command, char *comand);
void	ejecutar(char *envp[], char *path, char *comando);
void	first_process(t_pipex *pipexx);
void	last_process(t_pipex *pipexx);
void	close_pipes(t_pipex *pipexx);

#endif