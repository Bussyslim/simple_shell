#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

#define FALSE 0
#define TRUE 1
#define NEITHER 2
#define MATCH 3
#define PREFIX 4
#define EXIT_SHELL 5
#define SKIP_FORK 6
#define DO_EXECVE 7

/**
 * struct Alias - singly linked list
 * @name: name of alias
 * @value: command that alias calls
 * @next: points to next node
 */
typedef struct Alias
{
	char *name;
	char *value;
	struct Alias *next;
} alias;

extern char **environ;

extern int status;

extern int line_num;

extern char *shell_name;

int command_manager(char **args);

int built_ins(char **args);

int and_or(char **args, char operator, int last_compare);

char *check_command(char **args);

int execute_command(char **args);

char *input_san(char *old_buf, size_t *old_size);

int input_err_check(char *ptr);

void err_message(char *arg0, char *arg1);

int _getline(char **line_ptr, size_t *n, int file);

char *check_for_vars(char *arg);

char *_strdup(char *src);

char *str_concat(char *s1, char *s2);

int str_compare(char *s1, char *s2, int pref_or_match);

char *get_array_element(char **array, char *element_name);

char **make_array(char *str, char delim, char **if_sep);

int list_len(char **list, char *entry);

char **array_cpy(char **old_array, int new_size);

int free_array(char **args);

int _setenv(const char *name, const char *value);

int _unsetenv(const char *name);

int change_dir(char *name);

int alias_func(char **args, int free);

int free_aliases(alias *alias_ptr);

int check_if_alias(char **args, alias *alias_ptr);

int print_aliases(alias *alias_ptr);

int print_alias_value(char *arg, alias *alias_ptr);

int set_alias_value(char *arg, alias *alias_ptr, char *new_value);

int print_env(void);

char *_itoa(int n);

int _atoi(char *s);

/**
 * struct variables - variables
 * @av: command line arguments
 * @buffer: buffer of command
 * @env: environment variables
 * @count: count of commands entered
 * @argv: arguments at opening of shell
 * @status: exit status
 * @commands: commands to execute
 */

typedef struct variables
{
	char **av;
	char *buffer;
	char **env;
	size_t count;
	char **argv;
	int status;
	char **commands;
} vars_t;

/**
 * struct builtins - struct for the builtin functions
 * @name: name of builtin command
 * @f: function for corresponding builtin
 */
typedef struct builtins
{
	char *name;
	void (*f)(vars_t *);
} builtins_t;

char **make_env(char **env);
void free_env(char **env);

ssize_t _puts(char *str);
char *_strdup(char *strtodup);
int _strcmpr(char *strcmp1, char *strcmp2);
char *_strcat(char *strc1, char *strc2);
unsigned int _strlen(char *str);

char **tokenize(char *buffer, char *delimiter);
char **_realloc(char **ptr, size_t *size);
char *new_strtok(char *str, const char *delim);

void (*check_for_builtins(vars_t *vars))(vars_t *vars);
void new_exit(vars_t *vars);
void _env(vars_t *vars);
void new_setenv(vars_t *vars);
void new_unsetenv(vars_t *vars);

void add_key(vars_t *vars);
char **find_key(char **env, char *key);
char *add_value(char *key, char *value);
int _atoi(char *str);

void check_for_path(vars_t *vars);
int path_execute(char *command, vars_t *vars);
char *find_path(char **env);
int execute_cwd(vars_t *vars);
int check_for_dir(char *str);

void print_error(vars_t *vars, char *msg);
void _puts2(char *str);
char *_uitoa(unsigned int count);

#endif /* _SHELL_H_ */
