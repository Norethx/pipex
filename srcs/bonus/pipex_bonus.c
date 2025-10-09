/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:50:14 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/09 19:56:57 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_fork	info;
	char	**args;
	int		i;

	if (argc < 5)
		return (0);
	info.argc_r = argc;
	info.fd[OUTFILE] = -1;
	if (ft_strcmp("here_doc", argv[1]))
		info.is_here_doc = 0;
	else
		info.is_here_doc = 1;
	if (control_gc(-1) == 1)
		return (1);
	open_files(&info, argv, argc, INFILE);
	args = control_args(argv, argc, &info);
	info.index = 0;
	i = control_fork(info, argv, args, envp);
	if (i != 0 && i != 1)
		control_gc(WEXITSTATUS(i));
	control_gc(i);
}

char	**control_args(char **argv, int argc, t_fork *info)
{
	int		i[2];
	char	**rtn;

	i[1] = 0;
	if (info->is_here_doc)
		i[0] = 3;
	else
		i[0] = 2;
	rtn = ft_gc_calloc_root(argc - i[0], sizeof(char *), GC_DATA, "args");
	if (!rtn)
		return (NULL);
	while (i[0] <= argc - 2)
		rtn[i[1]++] = argv[i[0]++];
	info->argc = i[1] - 1;
	return (rtn);
}

int	open_files(t_fork *info, char **argv, int argc, t_file f_open)
{
	if (!info->is_here_doc && f_open == OUTFILE)
		info->fd[OUTFILE] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT,
				0644);
	else if (f_open == OUTFILE)
		info->fd[OUTFILE] = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT,
				0644);
	if (info->fd[OUTFILE] == -1 && f_open == OUTFILE)
	{
		perror("");
		return (1);
	}
	if (info->is_here_doc && f_open == INFILE)
		create_heredoc(argv[2], info);
	else if (f_open == INFILE)
		info->fd[INFILE] = open(argv[1], O_RDONLY);
	if (info->fd[INFILE] == -1 && f_open == INFILE)
	{
		perror("");
		return (1);
	}
	return (0);
}

int	control_fork(t_fork info, char **argv, char **args, char **envp)
{
	char	**path;
	pid_t	*childs;

	childs = ft_gc_calloc_root(info.argc + 1, sizeof(int), GC_DATA, "child");
	path = find_path(envp);
	while (info.index <= info.argc)
	{
		if (info.index == info.argc && open_files(&info, argv, info.argc_r,
				OUTFILE))
			break ;
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
	if (childs[info.argc] == 0)
		return (1);
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
		control_gc(127);
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
	control_gc(1);
	return (1);
}
