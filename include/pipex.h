/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:49:07 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/08 23:19:26 by rgomes-d         ###   ########.fr       */
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
}			t_file;

typedef enum e_mode_close
{
	PARENT,
	CHILD,
	ALL,
}			t_mode_close;

typedef struct s_fork
{
	int		fd[2];
	int		pipefd[2];
	int		argc;
	int		status;
	int		index;
	int		old_stdout;
	int		is_here_doc;
}			t_fork;

#endif
