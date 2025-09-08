/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:21:38 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/08 16:44:33 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
//# include "parsing.h"
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <readline/readline.h>
# include <readline/history.h>

extern	int	g_exit_code;

typedef enum	e_type
{
	INFILE,
	OUTFILE,
	APPEND,
	HERE_DOC,
	WORD,
	PIPE,
	END
}	t_type;

typedef struct	s_redirect
{
	char			*file;
	struct s_redirect 	*next;
	int			fd;
	t_type			type;
}	t_redirect;

typedef struct	s_cmd
{
	struct	s_cmd	*previous;
	struct	s_cmd	*next;
	char		**args;
	t_redirect	*redir;
	int		fd_in;
	int		fd_out;
	int		fd[2];

}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*previous;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	t_cmd *top_cmd;
	t_env *top_env;
	struct s_token *top_tok;

} t_minishell;

/*function for getting env path the right way and execution*/
char	*search_path(char *str, t_env *envp);

/*function opening files*/
void	opening_infile(char *argv);
void	opening_outfile(char *argv);

/*function for cmd lst*/
t_cmd	*create_node_cmd(char **str);
void    push_cmd(t_cmd **top, t_cmd *new);
t_cmd	*get_last_cmd(t_cmd **top);
void	push_back_cmd(t_cmd **top, t_cmd *new);
int     cmd_lst_size(t_cmd *top);
void	pop_cmd(t_cmd **top);
void	destructor_cmd(t_cmd **top);

/*function for env lst*/
t_env	*create_node_env(char *key_env, char *value_env);
void	push_env(t_env **top_env, t_env *new);
t_env	*get_last_env(t_env **top_env);
void	push_back_env(t_env **top_env, t_env *new);
void	pop_env(t_env **top_env);
int	env_lst_size(t_env *top_env);
void	destructor_env(t_env **top_env);
char	*research_key_env(t_env *top_env, char *search);
char	**lst_to_tab_env(t_env *top);
int	set_up_env(t_env **top_env, char **envp);
char	**split_env(char *envp);

/*function for redirection lst*/
void	pop_redir(t_redirect **top);
void	destructor_redir(t_redirect **top);
void	push_back_redir(t_redirect **top, t_redirect *redirect);
int	redir_lst_size(t_redirect *top);
t_redirect	*get_last_redir(t_redirect **top);
t_redirect	*create_node_redir(char *str, t_type type);


/*functions for execution*/
int	execution_node(char **str, t_minishell *minishell);
void	execute_child(t_cmd *emp, t_minishell *minishell);
int	multipipe_cmd(t_minishell *minishell);
void	close_fd_parent(t_cmd *top_stack);
void	command_redirect(t_cmd *top_stack);
int	launch_builtin(char **str, t_minishell *minishell);
int	is_it_builtin(char *str);
int	one_command(t_minishell *minishell);
void	executor(t_minishell *minishell);
int	one_command_execve(t_minishell *minishell);
void	fail_previous(void);
void	close_previous(t_cmd *top_stack);

/*redirection verification*/
int	redirection_verification(t_cmd **top_stack);
void	close_redir_temp(t_cmd *temp);
void	close_fd_error(t_cmd **top_stack);
int	infile_redirection(t_redirect *redir, t_cmd **top_stack);
int	outfile_append_redirection(t_redirect *redir, t_cmd **top_stack, int type);

/*BUILTIN*/
void	env(t_env *top_env);
void	pwd(void);
void	unset(t_minishell *minishell, char *key);
int	my_echo(char **argv);
int	cd(char **argv, t_minishell *minishell);
int	export(char **str, t_minishell *minishell);

/*heredoc document*/
int	here_doc_management(t_cmd *top_cmd);
int	heredoc_input(t_redirect *temp, t_cmd *top_cmd);

/*signals*/
void	sigint_handler(int sig);
void	sigint_handler2(int sig);
void	sigint_heredoc(int sig);

/*function utils*/
char	*join_path(char *str, char *str2, char sep);
void	ft_close_fd(int *fd);
void	banner(void);
void	active_echoctl(void);
void	remove_echoctl(void);
t_env *research_node_env(t_env *top_env, char *search);

#endif
