/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:49:07 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/02 16:42:51 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#define PATH = "/usr/bin/which"

# include "libft.h"
# include "sys/wait.h"

typedef enum e_file
{
	INFILE,
	OUTFILE,
}	t_file;

#endif
