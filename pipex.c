/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:50:14 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/06 21:19:03 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int		control_gc(int op);
int		handle_files(t_fork *info, char **argv);
int		open_files(t_fork *info, char **argv);
int		control_fork(t_fork info, char **argv, char **envp);
int		close_fds(t_fork info, int mode);
char	**find_path(char **envp);
char	*find_executable(char *arg, char **path);
int		handle_child(t_fork info, char *argv, char **path, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_fork	info;
	int		i;

	i = 1;
	if (argc < 5)
		return (0);
	info.index = 0;
	info.argc = argc;
	if (handle_files(&info, argv) == -1)
		return (1);
	if (control_gc(1) == 1)
		return (1);
	if (!control_fork(info, argv, envp))
		;
	control_gc(0);
}

int	handle_files(t_fork *info, char **argv)
{
	open_files(info, argv);
	if (pipe(info->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (info->fd[INFILE] == -1)
		return (1);
	return (0);
}

int	open_files(t_fork *info, char **argv)
{
	info->fd[OUTFILE] = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (info->fd[OUTFILE] == -1)
	{
		perror("open");
		return (1);
	}
	info->fd[INFILE] = open(argv[1], O_RDONLY);
	if (info->fd[INFILE] == -1)
	{
		perror("open");
		return (1);
	}
	return (0);
}

int	control_fork(t_fork info, char **argv, char **envp)
{
	int		i;
	char	**path;

	i = 2;
	path = find_path(envp);
	while (i <= info.argc - 2)
	{
		info.child = fork();
		if (info.child == -1)
			return (1);
		info.index = i;
		if (info.child == 0)
			handle_child(info, argv[i], path, envp);
		else
		{
			waitpid(info.child, &info.status, 0);
			if (info.status == EXIT_FAILURE)
				return (1);
			i++;
		}
	}
	close_fds(info, C_CIN);
	return (0);
}

int	handle_child(t_fork info, char *argv, char **path, char **envp)
{
	char	*exec_path;
	char	**newargv;

	newargv = ft_split(argv, ' ');
	if (info.index == 2)
	{
		if (dup2(info.fd[INFILE], STDIN_FILENO) == -1)
			ft_printf("oi");
	}
	else if (dup2(info.pipefd[0], STDIN_FILENO) == -1)
		perror("dup2");
	exec_path = find_executable(newargv[0], path);
	if (!exec_path)
	{
		ft_putstr_fd("oi", 2);
		exit(EXIT_FAILURE);
	}
	if (info.index == (info.argc - 2))
	{
		close(info.fd[0]);
		// close(info.fd[1]);
		if (dup2(info.fd[OUTFILE], STDOUT_FILENO) == -1)
			perror("dup2");
	}
	else if (dup2(info.pipefd[1], STDOUT_FILENO) == -1)
		perror("dup2");
	if (info.index == (info.argc - 2))
		close_fds(info, C_COUT_END);
	else
		close_fds(info, C_COUT);
	execve(exec_path, newargv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}
char	*find_executable(char *arg, char **path)
{
	int		i;
	char	*str;

	i = 0;
	if (ft_strchr(arg, '/'))
	{
		if (access(arg, X_OK) == 0)
			return (arg);
	}
	else
	{
		arg = ft_gcfct_register_root(ft_strjoin("/", arg), GC_DATA, "temp");
		while (path[i])
		{
			str = ft_gcfct_register_root(ft_strjoin(path[i], arg), GC_DATA,
					"temp");
			if (access(str, X_OK) == 0)
				return (str);
			i++;
		}
	}
	return (NULL);
}

char	**find_path(char **envp)
{
	int		i;
	char	**path;

	path = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH", 4))
		{
			path = ft_gcfct_arr_register_root((void *)ft_split(envp[i], '='),
					GC_DATA, "temp");
			path = ft_gcfct_arr_register_root((void *)ft_split(path[1], ':'),
					GC_DATA, "PATH");
			break ;
		}
		i++;
	}
	ft_gc_del_root("temp");
	ft_gc_collect();
	return (path);
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
// 	if (pid == 0)
// 		control_gc(2);
// 	if (pid != 0)
// 		control_gc(0);
// 	return (0);
// }

int	close_fds(t_fork info, int mode)
{
	if (mode == C_CIN)
	{
		close(info.fd[0]);
		close(info.pipefd[1]);
	}
	else if (mode == C_COUT)
	{
		close(info.fd[0]);
		close(info.fd[1]);
		close(info.pipefd[0]);
	}
	else if (mode == C_COUT_END)
	{
		close(info.fd[0]);
		close(info.fd[1]);
		close(info.pipefd[0]);
		close(info.pipefd[1]);
	}
	return (0);
}

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
