#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };

	if (ac == 2)
	{
		info->readfd = open_file(info, av[1], 0);
		if (info->readfd == -1)
		{
			free_info(info, 1);
			exit(info->err_num);
		}
	}
	populate_env_list(info);
	read_history(info);
	read_startup_file(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}
