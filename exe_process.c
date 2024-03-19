/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-m <carlos-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:24:51 by carlos-m          #+#    #+#             */
/*   Updated: 2024/02/28 13:24:52 by carlos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ejecutar(char *envp[], char *path, char *comando)
{
	char	**arguments;

	arguments = ft_split(comando, ' ');
	execve(path, arguments, envp);
	write(2, "Error, comando no encontrado\n", 29);
	exit(127);
}

void	first_process(t_pipex *pipexx)
{
	char	*path;
	int		infile;

	infile = open((pipexx->argv)[1], O_RDONLY);
	if (infile < 0)
	{
		write(1, "Error, no such file or directory\n", 33);
		exit(1);
	}
	dup2((pipexx->pipes)[(pipexx->argc) - 5][1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(infile);
	close_pipes(pipexx);
	path = find_path(pipexx->env, (pipexx->argv)[2]);
	ejecutar(pipexx->env, path, pipexx->argv[2]);
	exit(0);
}

void	last_process(t_pipex *pipexx)
{
	char	*path;
	int		outfile;

	outfile = open((pipexx->argv)[pipexx->argc - 1], \
	O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		write(1, "Error, outfile error\n", 21);
		exit(1);
	}
	dup2(pipexx->pipes[0][0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close_pipes(pipexx);
	path = find_path(pipexx->env, (pipexx->argv)[pipexx->argc - 2]);
	ejecutar(pipexx->env, path, (pipexx->argv)[pipexx->argc - 2]);
	exit(0);
}
