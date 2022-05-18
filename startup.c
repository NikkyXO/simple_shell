#include "shell.h"

/**
 * open_file - opens a file descriptor, exits on failure
 * @info: the parameter struct
 * @name: name of file to open
 * @silent: if true don't print errors
 * Return: fd of file
 */
int open_file(info_t *info, char *name, int silent)
{
	int fd;

	fd = open(name, O_RDONLY);
	if (fd == -1 && !silent)
	{
		if (errno == EACCES)
			info->err_num = 126;
		if (errno == ENOENT)
		{
			_eputs("./hsh");
			_eputs(": 0: Can't open ");
			_eputs(name);
			_eputchar('\n');
			_eputchar(BUF_FLUSH);
			info->err_num = 127;
		}
		return (-1);
	}
	return (fd);
}

/**
 * read_startup_file - reads the startup file
 * @info: the parameter struct
 */
void read_startup_file(info_t *info)
{
	char *buf;

	if (!_getenv(info, "HOME="))
		return;
	buf = malloc(_strlen(_getenv(info, "HOME=")) +
		_strlen(STARTUP_FILE) + 2);
	if (!buf)
		return;
	*buf = 0;
	sprintf(buf, "%s/%s", _getenv(info, "HOME="), STARTUP_FILE);
	info->startup_fd = open_file(info, buf, 1);
	free(buf);
}
