/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:50:14 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/08 23:40:50 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int		control_gc(int op);
int		handle_pipes(t_fork *info);
int		open_files(t_fork *info, char **argv, int argc);
int		control_fork(t_fork info, char **argv, char **envp);
int		close_fds(t_fork info, int mode);
char	**find_path(char **envp);
char	*find_executable(char *arg, char **path);
int		handle_child(t_fork info, char *argv, char **path, char **envp);
char	**control_args(char **argv, int argc, t_fork *info);

int	main(int argc, char **argv, char **envp)
{
	t_fork	info;
	char	**args;
	int		i;

	if (argc < 5)
		return (0);
	if (ft_strcmp("here_doc", argv[1]))
		info.is_here_doc = 0;
	else
		info.is_here_doc = 1;
	open_files(&info, argv, argc);
	if (control_gc(1) == 1)
		return (1);
	args = control_args(argv, argc, &info);
	i = control_fork(info, args, envp);
	if (WEXITSTATUS(i) != 127 && i != 0)
		control_gc(2);
	control_gc(0);
	if (WEXITSTATUS(i) == 127)
		return (WEXITSTATUS(i));
}

char	**control_args(char **argv, int argc, t_fork *info)
{
	int	i[2];
	char	**rtn;

	i[1] = 0;
	if (ft_strnstr("here_doc",argv[1],10))
		i[0] = 3;
	else
		i[0] = 2;
	rtn = ft_gc_calloc_root(argc - i[0], sizeof(char *),GC_DATA,"args");
	if (!rtn)
		return (NULL);
	while(i[0] <= argc - 2)
		rtn[i[1]++] = argv[i[0]++];
	info->argc = i[1] - 1;
	info->index = 0;
	return (rtn);
}

int	handle_pipes(t_fork *info)
{
	if (info->index == 0)
		info->old_stdout = info->fd[INFILE];
	else
		info->old_stdout = info->pipefd[0];
	if (pipe(info->pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

// void	create_heredoc(char *eof, t_fork *info)
// {
// 	if (pipe(info->pipefd) == -1)
// 	{
// 			perror("pipe");
// 			return (1);
// 	}
// 	info->child = fork();
// 	if (info->child == 0)
// 	{
// 		close (info->pipefd[0]);
// 		heredoc_child(eof, info);
// 	}
// 	else
// 	{
// 		close (info->pipefd[1]);
// 		waitpid(info->child, info->status, 0);
// 	}
// }
// void	heredoc_child(char *eof, t_fork *info)
// {
// 	char *str;

// 	dup2(info->pipefd[1], 1);
// 	eof = ft_strjoin(eof,"\n");
// 	str = ft_strdup("");
// 	while (ft_strcmp(eof, str))
// 	{
// 		ft_printf("%s",str);
// 		free(str);
// 		str = get_next_line(0);
// 	}
// 	exit(1);
// }

int	open_files(t_fork *info, char **argv, int argc)
{
	if (!info->is_here_doc)
		info->fd[OUTFILE] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		info->fd[OUTFILE] = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
	if (info->fd[OUTFILE] == -1)
	{
		perror("open");
		info->fd[OUTFILE] = 1;
	}
	info->fd[INFILE] = open(argv[1], O_RDONLY);
	if (info->fd[INFILE] == -1)
		perror("open");
	return (0);
}

int	control_fork(t_fork info, char **args, char **envp)
{
	char	**path;
	pid_t		*childs;

	childs = ft_gc_calloc_root(info.argc + 1, sizeof(int), GC_DATA, "child");
	path = find_path(envp);
	while (info.index <= info.argc)
	{
		handle_pipes(&info);
		childs[info.index] = fork();
		if (childs[info.index] == -1)
			return (1);
		if (childs[info.index] == 0)
			handle_child(info, args[info.index], path, envp);
		close_fds(info, PARENT);
		info.index++;
	}
	info.index = 0;
	while (info.index <= info.argc)
		waitpid(childs[info.index++], &info.status, 0);
	return (info.status);
}

int	handle_child(t_fork info, char *argv, char **path, char **envp)
{
	char	*exec_path;
	char	**newargv;

	newargv = ft_gcfct_arr_register_root((void **)ft_split(argv, ' '), GC_DATA,
		"split");
	exec_path = find_executable(newargv[0], path);
	if (!exec_path || info.old_stdout == -1)
		close_fds(info, ALL);
	if (!exec_path || info.old_stdout == -1)
		control_gc(3);
	if (dup2(info.old_stdout, STDIN_FILENO) == -1)
		perror("dup2");
	if (info.index == info.argc)
	{
		if (dup2(info.fd[OUTFILE], STDOUT_FILENO) == -1)
			perror("dup2");
	}
	else if (dup2(info.pipefd[1], STDOUT_FILENO) == -1)
		perror("dup2");
	close_fds(info, CHILD);
	execve(exec_path, newargv, envp);
	perror("execve");
	control_gc(2);
	return (1);
}

char	*find_executable(char *arg, char **path)
{
	int		i;
	char	*str;

	i = 0;
	if (!arg)
		return (NULL);
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

int	close_fds(t_fork info, int mode)
{
	if (mode == ALL)
		if (info.fd[INFILE] != -1)
			close(info.fd[INFILE]);
	if (mode == PARENT)
	{
		close(info.pipefd[1]);
		if (info.old_stdout != -1)
			close(info.old_stdout);
	}
	else if (mode == CHILD || mode == ALL)
	{
		close(info.fd[OUTFILE]);
		close(info.pipefd[0]);
		close(info.pipefd[1]);
		close(info.old_stdout);
	}	
	if (mode == PARENT && info.index == info.argc)
	{
		close(info.pipefd[0]);
		close(info.fd[OUTFILE]);
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
		exit(EXIT_FAILURE);
	}
	else if (op == 3)
	{
		ft_gc_end();
		exit(127);
	}
	else
		ft_gc_end();
	return (0);
}
