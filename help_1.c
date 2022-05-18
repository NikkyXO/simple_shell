#include "shell.h"

/**
 * help - returns information about args
 * Return: 0 on success
 */
void help(void)
{
	printf("ANT hsh, version 1.2.01-release (x86_64-pc-linux-gnu)\n"
	       "These shell commands are defined internally.  Type `help' to see this list.\n"
	       "Type `help name' to find out more about the function `name'.\n"

	       "cd\tenv\texit\nhelp\thistory\tpwd");
}

/**
 * help_cd - prints help info for cd built-in
 * @info: struct with flags
 * Return: void
 */
void help_cd(info_t *info)
{
	if (info->help && _strcmp(info->help, "s") == 0)
		printf("cd: cd [-L|[-P [-e]] [-@]] [dir]\n");
	else if (info->help && _strcmp(info->help, "d") == 0)
		printf("cd - Change the shell working directory.\n");
	else
	{
		printf("cd: cd [-L|[-P [-e]] [-@]] [dir]\n");
		printf("    Change the shell working directory.\n\n");
		printf("    Change the current directory to DIR.  ");
		printf("The default DIR is the value of the\n    HOME shell variable.\n\n");
		printf("    The variable CDPATH defines the search path for the directory ");
		printf("containing\n    DIR.  Alternative directory names in CDPATH are ");
		printf("separated by a colon (:).\n");
		printf("    A null directory name is the same as the current directory.");
		printf("  If DIR begins\n"
		       "    with a slash (/), then CDPATH is not used.\n\n");
		printf("    If the directory is not found, and the shell option");
		printf("`cdable_vars' is set,\n    the word is assumed to be  a variable ");
		printf("name.  ");
		printf("If that variable has a value,\n    its value is used for DIR.\n\n");
		printf("    Options:\n"
		       "        -L\tforce symbolic links to be followed: resolve symbolic links in\n"
		       "        DIR after processing instances of `..'\n");
		printf("        -P\tuse the physical directory structure without following");
		printf(" symbolic\n        links: resolve symbolic links in DIR before ");
		printf("processing instances\n        of `..'\n");
		printf("        -e\tif the -P option is supplied, and the current working ");
		printf("directory\n        cannot be determined successfully, exit with a ");
		printf("non-zero status\n");
		printf("        -@  on systems that support it, present a file with ");
		printf("extended attributes\n"
		       "            as a directory containing the file attributes\n\n");
		printf("    The default is to follow symbolic links, as if `-L' were ");
		printf("specified.\n");
		printf("    Exit Status:\n");
		printf("    Returns 0 if the directory is changed, and if $PWD is set ");
		printf("successfully when\n    -P is used; non-zero otherwise.\n");
	}
	info->help = NULL;
}

/**
 * help_exit - prints help for exit built-in
 * @info: struct with flags
 * Return: void
 */
void help_exit(info_t *info)
{
	if (info->help && _strcmp(info->help, "s") == 0)
		printf("exit: exit [n]\n");
	else if (info->help && _strcmp(info->help, "d") == 0)
		printf("exit - Exit the shell.\n");
	else
	{
		printf("exit: exit [n]\n    Exit the shell.\n\n");
		printf("\n    Exits the shell with a status of N. ");
		printf("If N is omitted, the exit status\n");
		printf("    that of the last arg executed.\n");
	}
	info->help = NULL;
}

/**
 * help_help - prints help for help
 * @info: struct with command
 * Return: void
 */
void help_help(info_t *info)
{
	if (info->help && _strcmp(info->help, "s") == 0)
		printf("help: help [-dms] [pattern ...]\n");
	else if (info->help && _strcmp(info->help, "d") == 0)
		printf("help - Display information about builtin commands.\n");
	else
	{
		printf("help: help [-dms] [pattern ...]\n");
		printf("    Display information about builtin commands.\n\n");
		printf("    Displays brief summaries of builtin commands.  If PATTERN is\n");
		printf("    specified, gives detailed help on all commands matching ");
		printf("PATTERN,\n");
		printf("    otherwise the list of help topics is printed.\n\n");
		printf("    Options:\n");
		printf("      -d\toutput short description for each topic\n");
		printf("      -m\tdisplay usage in pseudo-manpage format\n");
		printf("      -s\toutput only a short usage synopsis for each topic ");
		printf("matching\n");
		printf("        PATTERN\n\n");
		printf("    Arguments:\n");
		printf("      PATTERN\tPattern specifiying a help topic\n\n");
		printf("    Exit Status:\n");
		printf("    Returns success unless PATTERN is not found ");
		printf("or an invalid option is given.\n");
	}
	info->help = NULL;
}

/**
 * help_history - displays help info about history built-in
 * @info: struct with command
 * Return: void
 */
void help_history(info_t *info)
{
	if (info->help && _strcmp(info->help, "s") == 0)
	{
		printf("history: history [-c] [-d offset] [n] or history -anrw ");
		printf("[filename] or history -ps arg [arg...]\n");
	}
	else if (info->help && _strcmp(info->help, "d") == 0)
		printf("history - Display or manipulate the history list.\n");
	else
	{
		printf("history: history [-c] [-d offset] [n] or history -anrw [filename] ");
		printf("or history -ps arg [arg...]\n");
		printf("    Display or manipulate the history list.\n\n");
		printf("    Display the history list with line numbers, prefixing each ");
		printf("modified\n"
		       "    entry with a `*'.  An argument of N lists only the last N entries.\n\n");
		printf("    If FILENAME is given, it is used as the history file.  ");
		printf("    Exit Status:\n    Returns success unless an invalid option is ");
		printf("given or an error occurs.\n");
	}
	info->help = NULL;
}
