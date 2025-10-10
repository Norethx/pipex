/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:18:32 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/10 16:58:03 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_heredoc(char *eof, t_fork *info)
{
	char	*str;
	int		tmp;

	tmp = open("/tmp", O_RDWR | __O_TMPFILE, 0667);
	if (tmp == -1)
		return ;
	eof = ft_gcfct_register_root(ft_strjoin(eof, "\n"), GC_DATA, "temp");
	ft_printf("> ");
	while (1)
	{
		str = ft_gcfct_register_root(get_next_line(STDIN_FILENO), GC_DATA,
				"temp");
		if (str && !ft_strcmp(str, eof))
			break ;
		else if(str && ft_putstr_fd(str, tmp))
			ft_printf("> ");
	}
	eof = ft_gcfct_register_root(ft_itoa(tmp), GC_DATA, "temp");
	str = ft_gcfct_register_root(ft_strjoin("/proc/self/fd/", eof), GC_DATA,
			"temp");
	if (eof && str)
		info->fd[INFILE] = open(str, O_RDONLY);
	close(tmp);
}
