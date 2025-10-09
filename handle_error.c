/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-d <rgomes-d@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:50:52 by rgomes-d          #+#    #+#             */
/*   Updated: 2025/10/08 22:34:36 by rgomes-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	heredoc_child(char *eof, t_fork *info);
int		create_heredoc(char *eof, t_fork *info);

int	main (void)
{
	t_fork info;
	create_heredoc("eof", &info);
}

int	create_heredoc(char *eof, t_fork *info)
{
	if (pipe(info->pipefd) == -1)
	{
			ft_printf("%s","oi1");
			perror("pipe");
			return (1);
	}
	info->child = fork();
	if (info->child == 0)
	{
		ft_printf("%s","oi2");
		close (info->pipefd[0]);
		heredoc_child(eof, info);
	}
	else
	{
		ft_printf("%s","oi3");
		close (info->pipefd[1]);
		waitpid(info->child, &info->status, 0);
	}
}
void	heredoc_child(char *eof, t_fork *info)
{
	char *str;

	//dup2(info->pipefd[1], 1);
	eof = ft_strjoin(eof,"\n");
	str = ft_strdup("");
	ft_printf(" %d \n",ft_strcmp(eof, str));
	while (ft_strcmp(eof, str) != 0)
	{
		ft_printf("%s",ft_strcmp(eof, str));
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	exit(0);
}
