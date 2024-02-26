/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-m <carlos-m@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 11:19:36 by carlos-m          #+#    #+#             */
/*   Updated: 2024/01/21 11:19:37 by carlos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *envp[], char *comand)
{
	int		i;
	char	**full_command;
	char	*path;
	char	**possible_paths;

	i = 0;
	if (!comand || comand[0] == '\0')
	{
		write (2, "Error, no command\n", 18);
		exit(1);
	}
	while (envp[i] && ft_strncmp("PATH", envp[i], 4) != 0)
		i++;
	if (!envp[i] || envp[i] == NULL)
		return (comand);
	full_command = ft_split(comand, ' ');
	path = envp[i] + 5;
	possible_paths = ft_split(path, ':');
	i = 1;
	path = ft_strjoin(ft_strjoin(possible_paths[0], "/"), full_command[0]);
	while (access(path, X_OK) != 0 && path != NULL)
	{
		free(path);
		if (possible_paths[i])
		{
			path = ft_strjoin(ft_strjoin(possible_paths[i], "/"), full_command[0]);
			i++;
		}
		else
			return (comand);
	}
	return (path);
}

void	ejecutar(char *envp[], char *path, char *comando)
{
	char	**arguments;

	arguments = ft_split(comando, ' ');
	execve(path, arguments, envp);	
	write(2, "Error, comando no encontrado\n", 29);
	exit(127);
}

void	first_process(char *name_inf, int pipe_tube[2], char **env, char *cmd)
{
	char	*path;
	int		infile;

	infile = open(name_inf, O_RDONLY); // Abro el infile
	if (infile < 0)
	{
		write(1, "Error, no such file or directory\n", 33);
		exit(1);
	}
	close(pipe_tube[0]);
	dup2(pipe_tube[1], STDOUT_FILENO);
	close(pipe_tube[1]);
	dup2(infile, STDIN_FILENO);
	close(infile);
	path = find_path(env, cmd);
	ejecutar(env, path, cmd);
	exit(0);
}

void	second_process(char *name_outf, int pipe_tube[2], char **env, char *cmd)
{
	char	*path;
	int		outfile;

	outfile = open(name_outf, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		write(1, "Error, outfile error\n", 21);
		exit(1);
	}
	close(pipe_tube[1]);
	dup2(pipe_tube[0], STDIN_FILENO);
	close(pipe_tube[0]);
	dup2(outfile, STDOUT_FILENO);
    close(outfile); 
	path = find_path(env, cmd);
	ejecutar(env, path, cmd);
	exit(0);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		pipe_tube[2];
	int		child1;
	int		child2;
	int		status;

	if (argc != 5)
	{
		write(1, "Error, you have to put infile, 2 commands ando outfile\n", 55);
		exit(1);
	}
	if (pipe(pipe_tube) == -1)
	{
		write(1, "Error, pipe error\n", 18);
		exit(1);
	}
	child1 = fork();
	if (child1 == 0)
		first_process(argv[1], pipe_tube, envp, argv[2]);
	child2 = fork();
	if (child2 == 0)
		second_process(argv[4], pipe_tube, envp, argv[3]);
	close(pipe_tube[0]);
	close(pipe_tube[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, &status, 0);
	return (WEXITSTATUS(status));
}