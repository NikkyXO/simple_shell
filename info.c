#include "shell.h"

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
	if (info->left_redirect_from_fd != HEREDOC_FD)
	{
		/* TODO where else? when to RESET? */
		info->left_redirect_from_fd = -1;
	}
	info->left_append = 0;
	info->right_redirect_from_fd = 1;
	info->right_redirect_to_fd = -1;
	info->right_append = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 * Return: 0 if no error else error code
 */
int set_info(info_t *info, char **av)
{
	int i = 0;

	info->err_num = 0;
	info->fname = av[0];
	if (info->arg)
	{
		parse_left_redirect(info);
		parse_right_redirect(info);
		if (info->left_redirect_from_fd == HEREDOC_FD)
		{
			if (!info->heredoc_cmd)
				info->heredoc_cmd = _strdup(info->arg);
		}
		handle_redirects(info);
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		replace_vars(info);
	}
	return (info->err_num);
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	restore_stdfd(info);
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (info->left_redirect_from_fd > 2)
	{
		close(info->left_redirect_from_fd);
		info->left_redirect_from_fd = -1;
	}
	if (info->right_redirect_to_fd > 2)
	{
		close(info->right_redirect_to_fd);
		info->right_redirect_to_fd = -1;
	}
	if (info->heredoc_txt && info->left_redirect_from_fd != HEREDOC_FD)
		bfree((void **)&info->heredoc_txt);
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		bfree((void **)&info->heredoc);
		bfree((void **)&info->heredoc_txt);
		bfree((void **)&info->heredoc_cmd);
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		__getline(-1);
		_putchar(BUF_FLUSH);
	}
}
/**
 * print_info - prints info_t struct
 * @info: struct address
 */
void print_info(info_t *info)
{
	int i = 0;

	printf("info->arg:[%s]\n", info->arg);
	printf("info->argv:%s\n", info->argv ? "" : "[(null)]");
	for (i = 0; info->argv && info->argv[i]; i++)
		printf("\tinfo->argv[%d]:[%s]\n", i, info->argv[i]);
	printf("info->path:[%s]\n", info->path);
	printf("info->argc:[%d]\n", info->argc);
	printf("info->line_count:[%d]\n", info->line_count);
	printf("info->err_num:[%d]\n", info->err_num);
	printf("info->status:[%d]\n", info->status);
	printf("info->fname:[%s]\n", info->fname);
	printf("info->env:[%p]\n", (void *)info->env);
	printf("info->cmd_buf:[%p]\n", (void *)info->cmd_buf);
	printf("info->*cmd_buf:[%s]\n",
	       info->cmd_buf ? *(info->cmd_buf) : "NONE");

	printf("info->left_redirect_from_fd:[%d]\n", info->left_redirect_from_fd);
	printf("info->right_redirect_from_fd:[%d]\n", info->right_redirect_from_fd);
	printf("info->right_redirect_to_fd:[%d]\n", info->right_redirect_to_fd);
	printf("info->left_append:[%d]\n", info->left_append);
	printf("info->heredoc:[%s]\n", info->heredoc);
	printf("info->heredoc_txt:[%s]\n", info->heredoc_txt);
	printf("info->heredoc_cmd:[%s]\n", info->heredoc_cmd);
	printf("pipefd:[%d][%d]\n", info->pipefd[0], info->pipefd[1]);
	printf("==========================\n");
}
