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

int	main(int argc, char *argv[], char *envp[])
{
	int		infile;
	int		outfile;
	int		pipe_tube[2];
	char	*path;
	int		child1;
	int		child2;
	int		status;

	if (argc != 5)
	{
		write(1, "Error, insuficient arguments\n", 29);
		exit(1);
	}
	if (pipe(pipe_tube) == -1)
	{
		write(1, "Error, pipe error\n", 18);
		exit(1);
	}
	child1 = fork();
	if (child1 == 0) // proceso hijo1
	{
		close(pipe_tube[0]);
		dup2(pipe_tube[1], STDOUT_FILENO);
		close(pipe_tube[1]);
		infile = open(argv[1], O_RDONLY); // Abro el infile
		if (infile < 0)
		{
			write(1, "Error, infile error\n", 20);
			exit(1);
		}
		dup2(infile, STDIN_FILENO);
		close(infile);
		path = find_path(envp, argv[2]);
		ejecutar(envp, path, argv[2]);
		exit(0);
	}
	else if (child1 > 0) //proceso padre
	{
		child2 = fork();
		if (child2 == 0) // Proceso hijo2
		{
			close(pipe_tube[1]);
			dup2(pipe_tube[0], STDIN_FILENO);
			close(pipe_tube[0]);
			outfile = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0644); // Abro el outfile
			if (outfile < 0)
			{
				write(1, "Error, outfile error\n", 21);
				exit(1);
			}
			dup2(outfile, STDOUT_FILENO);
            close(outfile); 
			path = find_path(envp, argv[3]);
			ejecutar(envp, path, argv[3]);
			exit(0);
		}
		else if (child2 > 0) // Proceso padre
		{
			close(pipe_tube[0]);
			close(pipe_tube[1]);
		}
		else
		{
			write(1, "Error al crear el segundo proceso hijo\n", 39);
			exit(1);
		}
	}
	else
	{
		write(2, "Error al crear el proceso hijo\n", 31);
		exit(1);
	}
	waitpid(child1, NULL, 0);
	waitpid(child2, &status, 0);
	return (WEXITSTATUS(status));
}