/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigals_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:09:11 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/07 17:09:12 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Signal handler specifically for heredoc.
** Updates global status to 130, writes a newline, and closes stdin
** to force readline to return NULL and break the loop.
** @param signal: The signal number received.
*/
static void	signal_heredoc(int signal)
{
	(void)signal;
	g_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

/*
** Configures signal handlers for the heredoc phase.
** SIGINT (Ctrl+C): handled by signal_heredoc.
** SIGQUIT (Ctrl+\): ignored.
*/
void	set_signal_heredoc(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_heredoc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
}
