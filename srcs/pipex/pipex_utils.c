/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:20:45 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/09 19:59:56 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	perror("");
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
	if (op == -1)
		return (ft_gc_init());
	else
	{
		ft_gc_end();
		exit(op);
	}
	return (0);
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
