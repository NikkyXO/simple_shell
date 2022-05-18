#include "shell.h"

/**
 * _myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = _erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _mycd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
			    chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
		    chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: 0 on success, 1 on failure
 */
int _myhelp(info_t *info)
{
	char **arg;
	char *builtin;

	arg = info->argv;
	if (arg[1] == NULL)
	{
		help();
		return (0);
	}
	builtin = help_flag_check(info, arg);
	if (!builtin)
		return (0);
	if (_strcmp(builtin, "alias") == 0)
		help_alias(info);
	else if (_strcmp(builtin, "cd") == 0)
		help_cd(info);
	else if (_strcmp(builtin, "echo") == 0)
		help_echo(info);
	else if (_strcmp(builtin, "exit") == 0)
		help_exit(info);
	else if (_strcmp(builtin, "help") == 0)
		help_help(info);
	else if (_strcmp(builtin, "history") == 0)
		help_history(info);
	else if (_strcmp(builtin, "pwd") == 0)
		help_pwd(info);
	else
	{
		printf("-bash: help: no help topics match `%s'.", builtin);
		printf("  Try `help help' or `man -k %s' or `info %s'.\n", builtin, builtin);
		info->status = 1;
		return (1);
	}
	info->status = 0;
	return (0);
}

/**
 * help_flag_check - checks if there is valid flag on help built-in
 * @info: struct containing commands from line
 * @arg: arguments entered when using help
 * Return: builtin to search for
 */
char *help_flag_check(info_t *info, char **arg)
{
	char *builtin = arg[1];
	int i = 1, j = 0;

	if (arg[1][j] == '-')
	{
		for (j = 1; j < _strlen(arg[i]); j++)
		{
			if (arg[1][j] == 's')
				info->help = "s";
			if (arg[1][j] == 'd')
			{
				info->help = "d";
				break;
			}
		}
		for (j = 1; j < _strlen(arg[1]); j++)
		{
			if (arg[1][j] != 's' &&
			    arg[1][j] != 'd' &&
			    arg[1][j] != 'm')
			{
				printf("-hsh: help: %c: invalid option\n", arg[i][j]);
				printf("help: usage: help [-dms] [pattern ...]\n");
				info->help = NULL;
				return (NULL);
			}
		}
		return (arg[2]);
	}
	else
		return (builtin);
}
