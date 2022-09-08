#include "shell.h"

int status;

/**
 * change_dir - changes the current working directory
 * @name: name of directory to change to
 *
 * Return: 0 if successful
 */
int change_dir(char *name)
{
	char *home;
	char *pwd;
	char old_path_buffer[PATH_MAX];
	char new_path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	getcwd(old_path_buffer, buf_size);

	if (name == NULL)
	{
		home = get_array_element(environ, "HOME=");
		if (home == NULL)
		{
			status = 2;
			err_message("cd", name);
			return (SKIP_FORK);
		}

		home += 5;

		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);
	}
	else if (str_compare("-", name, MATCH) == TRUE)
	{
		pwd = get_array_element(environ, "OLDPWD=");
		if (pwd == NULL)
		{
			status = 2;
			err_message("cd", name);
			return (SKIP_FORK);
		}

		pwd += 7;

		i = chdir((const char *)pwd);
		if (i != -1)
		{
			write(STDOUT_FILENO, pwd, _strlen(pwd));
			write(STDOUT_FILENO, "\n", 1);
			_setenv("PWD", (const char *)pwd);
		}
	}
	else if (name != NULL)
	{
		i = chdir((const char *)name);
		if (i != -1)
			_setenv("PWD", getcwd(new_path_buffer, buf_size));
	}
	if (i == -1)
	{
		status = 2;
		err_message("cd", name);
		return (SKIP_FORK);
	}

	status = 0;
	_setenv("OLDPWD", (const char *)old_path_buffer);

	return (SKIP_FORK);
}

/**
 * alias_func - deals with command aliases
 * @args: arguments from command line
 * @to_free: indicated if aliases need to be freed (exiting shell);
 *
 * Return: TRUE if exiting, FALSE if the command is not "alias" or an alias,
 * SKIP_FORK if success
 */
int alias_func(char **args, int to_free)
{
	static alias head = {NULL, NULL, NULL};
	char *char_ptr;
	int no_error = TRUE;

	if (to_free == TRUE)
		return (free_aliases(head.next));

	if (str_compare("alias", *args, MATCH) != TRUE)
		return (check_if_alias(args, head.next));

	args++;

	if (*args == NULL)
		return (print_aliases(head.next));

	while (*args != NULL)
	{
		char_ptr = *args;
		while (*char_ptr != '\0' && *char_ptr != '=')
			char_ptr++;

		if (*char_ptr == '\0' || char_ptr == *args)
		{
			if (print_alias_value(*args, &head) == FALSE)
				no_error = FALSE;
		}
		else
		{
			*char_ptr = '\0';
			char_ptr++;
			set_alias_value(*args, &head, char_ptr);
			*(char_ptr - 1) = '=';
		}
		args++;
	}

	if (no_error == FALSE)
		return (SKIP_FORK);

	status = 0;
	return (SKIP_FORK);
}
