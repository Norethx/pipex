/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:49:07 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/06 17:54:45 by rgomes-d         ###   ########.fr       */
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
	C_CIN,
	C_COUT,
	C_COUT_END,
}			t_mode_close;

typedef struct s_fork
{
	int		fd[2];
	int		pipefd[2];
	int		argc;
	int		status;
	pid_t	child;
	int		index;
}			t_fork;

#endif
