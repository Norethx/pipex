/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:50:14 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/02 21:20:55 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	control_gc(int op);
int	handle_files(int *fd,char **argv);

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];
	int	pipefd[2];

	// pid_t	pid;
	pipefd[0] = 0;
	if (handle_files(&fd[2], argv) == 1)
		return(1);
	if (argc != 5)
		return (0);
	ft_printf("%d", fd[0]);
	ft_printf("%d", pipefd[0]);
	argv = NULL;
	envp = NULL;
	if (control_gc(1) == 1)
		return (1);
	control_gc(0);
}

int	handle_files(int *fd,char **argv)
{
	fd[OUTFILE] = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd[OUTFILE] == -1)
		return (1);
	fd[INFILE] = open(argv[4], O_RDONLY);
	if (fd[INFILE] == -1)
	{
		write(fd[OUTFILE], "0", 1);
		close(fd[OUTFILE]);
		return (1);
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	pid_t	pid;
// 	int		i;
// 	int		status;
// 	int		pipefd[2];
// 	char	*newargv[] = {"which", "which", NULL};
// 	char	**str;

// 	i = 0;
// 	pipe(pipefd);
// 	control_gc(1);
// 	// args = ft_gc_calloc_root(3, sizeof(char *), GC_DATA, "args");
// 	// args[0] = ft_gcfct_register_root(ft_strdup("which"), GC_DATA,
// 	// 		"name_program");
// 	// args[1] = ft_gcfct_register_root(ft_strdup("ls"), GC_DATA, "args_1");
// 	pid = fork();
// 	if (pid == -1)
// 		return (1);
// 	if (pid == 0)
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		execve("/usr/bin/which", newargv, NULL);
// 	}
// 	if (pid > 0)
// 	{
// 		waitpid(pid, &status, 0);
// 		close(pipefd[1]);
// 		ft_printf("%s\n", get_next_line(pipefd[0]));
// 		close(pipefd[0]);
// 	}
// 	argc = 1;
// 	argv[0] = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strnstr(envp[i], "PATH", 4))
// 		{
// 			str = ft_gcfct_arr_register_root((void *)ft_split(envp[i], '='),
// 					GC_DATA, "temp");
// 			str = ft_gcfct_arr_register_root((void *)ft_split(str[1], ':'),
// 					GC_DATA, "PATHS");
// 			ft_gc_del_root("temp");
// 			ft_gc_collect();
// 		}
// 		i++;
// 	}
// 	if (pid == 0)
// 		control_gc(2);
// 	if (pid != 0)
// 		control_gc(0);
// 	return (0);
// }

int	control_gc(int op)
{
	if (op == 1)
		return (ft_gc_init());
	else if (op == 2)
	{
		ft_gc_end();
		exit(0);
	}
	else
		ft_gc_end();
	return (0);
}
