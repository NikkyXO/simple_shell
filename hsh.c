#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (interactive(info))
			print_prompt(info);
		_eputchar(BUF_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			if (!set_info(info, av))
			{
				builtin_ret = find_builtin(info);
				if (builtin_ret == -1)
					find_cmd(info);
			} else
				builtin_ret = -3;
		}
		else if (interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret <= -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	if (info->heredoc)
		return (0);
	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		/*printf(CYN "WAIT DONE" RESL);*/
		if (info->pipefd[1] > 2)
		{
			/*printf(RED "CLOSING WRITE PIPE" RESL);*/
			close(info->pipefd[1]), info->pipefd[1] = 0;
		}

		info->left_redirect_from_fd = -1; /* RESET FD */
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}


/**
 * handle_redirects - handles all left/right redirect syscalls
 * @info: the parameter struct
 */
void handle_redirects(info_t *info)
{
	int pipefd[2];

	info->dup_stdin = dup(STDIN_FILENO);
	info->dup_stdout = dup(STDOUT_FILENO);
	if (info->left_redirect_from_fd == HEREDOC_FD)
	{
		if (pipe(pipefd) == -1)
			exit(1);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			exit(1);
		write(pipefd[1], info->heredoc_txt, _strlen(info->heredoc_txt));
		close(pipefd[0]);
		close(pipefd[1]);
	}
	else if (info->left_redirect_from_fd > -1)
	{
		if (dup2(info->left_redirect_from_fd, STDIN_FILENO) == -1)
		{
			/* TODO: error msg? */
			exit(1);
		}
	}
	if (info->right_redirect_to_fd > -1)
	{
		if (dup2(info->right_redirect_to_fd, info->right_redirect_from_fd) == -1)
		{
			/* TODO: error msg? */
			exit(1);
		}
	}
}
