#include "shell.h"

/**
 * open_pipe - handles inter process communication
 * @info: the parameter struct
 */
void open_pipe(info_t *info)
{
	/*printf(BLU "OPEN_PIPE()" RESL);*/
	if (!info->pipefd[0] && !info->pipefd[1])
	{
		if (pipe(info->pipefd) == -1)
			exit(1);
		/*printf(RED "NEW PIPE %d:%d" RESL, info->pipefd[0], info->pipefd[1]);*/
	}
	/* set right redirect to write-end of pipe */
	info->right_redirect_to_fd = info->pipefd[1];
}
