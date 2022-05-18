#include "shell.h"

/**
 * parse_left_redirect - parses redirect directives from input
 * @info: parameter struct
 * Return: void
 */
void parse_left_redirect(info_t *info)
{
	char *p = _strchrlast(info->arg, '<'), *file;
	int fd;

	if (!p) /* no redirect */
		return;

	file = p + 1;
	if (p > info->arg && *(p - 1) == '<')
	{
		info->left_append = 1;
		p--;
	}
	if (p > info->arg) /* if not start of string */
	{
		/* check if previous char is digit */
		if (*(p - 1) >= '0' && *(p - 1) <= '9')
		{
			p--;
			info->left_redirect_from_fd = *p - '0';
		}
	}
	fd = open_redirect(info, file, 1);
	if (fd == -1)
	{
		/* TODO: print message, set status, etc? */
		return;
	}
	info->left_redirect_from_fd = fd;
	*p = 0; /* insert null char to cut string b4 > */
}

/**
 * parse_right_redirect - parses redirect directives from input
 * @info: parameter struct
 * Return: void
 */
void parse_right_redirect(info_t *info)
{
	char *p = _strchrlast(info->arg, '>'), *file;
	int fd;

	if (!p) /* no redirect */
		return;

	file = p + 1;
	if (p > info->arg && *(p - 1) == '>')
	{
		info->right_append = 1;
		p--;
	}
	if (p > info->arg) /* if not start of string */
	{
		/* check if previous char is digit */
		if (*(p - 1) >= '0' && *(p - 1) <= '9')
		{
			p--;
			info->right_redirect_from_fd = *p - '0';
		}
	}
	fd = open_redirect(info, file, 0);
	if (fd == -1)
	{
		/* TODO: print message, set status, etc? */
		return;
	}
	info->right_redirect_to_fd = fd;
	*p = 0; /* insert null char to cut string b4 > */
}

/**
 * open_redirect - opens a fd for redirect writing
 * @info: parameter struct
 * @file: name of file
 * @left: 1 if left, 0 if right
 * Return: integer fd or -1
 */
int open_redirect(info_t *info, char *file, int left)
{
	int fd;
	char buf[256], *action;

	while (*file == ' ' || *file == '\t')
		file++;
	if (!*file)
	{
		print_error_noarg(info, "Syntax error: newline unexpected\n");
		return (-1);
	}
	if (left)
	{
		if (info->left_append)
		{
			info->heredoc = _strdup(file);
			if (!info->heredoc)
				exit(1);
			return (HEREDOC_FD);
		}
		fd = open(file, O_RDONLY);
	}
	else
		fd = open(file, O_CREAT | O_WRONLY |
			(info->right_append ? O_APPEND : O_TRUNC), 0666);
	if (fd == -1)
	{
		buf[0] = 0;
		info->line_count++;
		info->err_num = info->status = 2;
		action = left ? "open" : "create";
		if (errno == EACCES)
			sprintf(buf, "cannot %s %s: Permission denied\n", action, file);
		else if (errno == ENOENT)
			sprintf(buf, "cannot %s %s: No such file\n", action, file);
		print_error_noarg(info, buf);
	}
	return (fd);
}

/**
 * parse_heredoc - parses line of HEREDOC input into buffer
 * @info: the parameter struct
 * @buf: the getline buffer
 * @r: number of bytes read into getline buffer
 * Return: length of heredoc_cmd if end of heredoc else r
 */
size_t parse_heredoc(info_t *info, char **buf, size_t r)
{
	static char *heredoc_buf;
	static size_t heredoc_i, heredoc_len;
	size_t len;

	r++;
	if (!_strcmp(info->heredoc, *buf))
	{
		bfree((void **)buf);
		*buf = info->heredoc_cmd;
		info->heredoc_cmd = NULL;
		len = _strlen(*buf);
		info->heredoc_txt = heredoc_buf;
		heredoc_buf = NULL;
		heredoc_i = heredoc_len = 0;
		bfree((void **)&info->heredoc);
		return (len);
	}
	while (heredoc_len <= r + heredoc_i)
	{
		heredoc_buf = _realloc(heredoc_buf, heredoc_len,
			heredoc_len ? heredoc_len * 2 : STARTING_ARR_SIZE);
		if (!heredoc_buf)
			exit(1);
		heredoc_len <<= 1;
		if (!heredoc_len)
		{
			_memset(heredoc_buf, 0, STARTING_ARR_SIZE);
			heredoc_len = STARTING_ARR_SIZE;
		}
	}
	_strcat(heredoc_buf, *buf);
	_strcat(heredoc_buf, "\n");
	heredoc_i += r;
	return (r);
}

/**
 * restore_stdfd - restores stdin/out after redirect
 * @info: the parameter struct
 */
void restore_stdfd(info_t *info)
{
	_putchar(BUF_FLUSH);
	_eputchar(BUF_FLUSH);
	if (info->dup_stdin)
	{
		dup2(info->dup_stdin, STDIN_FILENO);
		close(info->dup_stdin);
		info->dup_stdin = 0;
	}
	if (info->dup_stdout)
	{
		dup2(info->dup_stdout, STDOUT_FILENO);
		close(info->dup_stdout);
		info->dup_stdout = 0;
	}
}
