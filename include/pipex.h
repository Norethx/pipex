/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:49:07 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/09 13:34:53 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define PATH = "/usr/bin/which"

# include "libft.h"
# include "sys/wait.h"

typedef enum e_file
{
	INFILE,
	OUTFILE,
}		t_file;

typedef enum e_mode_close
{
	PARENT,
	CHILD,
	ALL,
}		t_mode_close;

typedef struct s_fork
{
	int	fd[2];
	int	pipefd[2];
	int	argc;
	int	argc_r;
	int	status;
	int	index;
	int	old_stdout;
	int	is_here_doc;
}		t_fork;

int		control_gc(int op);
int		handle_pipes(t_fork *info);
int		open_files(t_fork *info, char **argv, int argc, t_file f_open);
int		control_fork(t_fork info, char **argv, char **args, char **envp);
int		close_fds(t_fork info, int mode);
char	**find_path(char **envp);
char	*find_executable(char *arg, char **path);
int		handle_child(t_fork info, char *argv, char **path, char **envp);
char	**control_args(char **argv, int argc, t_fork *info);
void	create_heredoc(char *eof, t_fork *info);

#endif
