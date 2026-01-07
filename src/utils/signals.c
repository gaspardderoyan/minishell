/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:34:59 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/07 17:37:38 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Signal handler for SIGINT (Ctrl+C).
** Updates global status and displays a new prompt line.
** @param signal: The signal number received.
*/
static void	signal_handler(int signal)
{
	g_status = signal;
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
** Configures signal handlers for the interactive shell.
** SIGINT (Ctrl+C): handled by signal_handler (new prompt line).
** SIGQUIT (Ctrl+\): ignored.
*/
void	set_signal_action(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_handler;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
}

/*
** Ignores SIGINT and SIGQUIT signals.
** Called when waiting for child processes to prevent shell interruption.
*/
void	ignore_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Resets signal handlers to default behavior (SIG_DFL).
** Called in child processes to restore normal signal handling.
*/
void	reset_signals_default(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
