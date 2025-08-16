#include "../../execute.h"

static char	*search_in_cwd(char *cmd)
{
	char	cwd[PATH_MAX];
	char	*tmp;
	char	*full_path;

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	tmp = ft_strjoin(cwd, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

typedef struct s_path
{
	char	**paths;
	char	*tmp;
	char	*full_path;
	int		i;
}	t_path;

char	*find_in_path(char *cmd, t_envlist *env)
{
	t_path	pt;

	while (env && ft_strcmp(env->variable, "PATH") != 0)
		env = env->next;
	if (!env || !env->value)
		return (search_in_cwd(cmd));
	pt.paths = ft_split(env->value, ':');
	if (!pt.paths)
		return (NULL);
	pt.i = 0;
	while (pt.paths[pt.i])
	{
		pt.tmp = ft_strjoin(pt.paths[pt.i], "/");
		if (!pt.tmp)
			return (free_argv(pt.paths), NULL);
		pt.full_path = ft_strjoin(pt.tmp, cmd);
		free(pt.tmp);
		if (!pt.full_path)
			return (free_argv(pt.paths), NULL);
		if (access(pt.full_path, X_OK) == 0)
			return (free_argv(pt.paths), pt.full_path);
		free(pt.full_path);
		pt.i++;
	}
	return (free_argv(pt.paths), NULL);
}

static char	*get_absolute_path(char *cmd, t_envlist *env)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (find_in_path(cmd, env));
}

static void	print_exec_error(char *cmd)
{
	if (ft_strchr(cmd, '/'))
		dprintf(STDERR_FILENO, "Migrane: %s: No such file or directory\n", cmd);
	else
		dprintf(STDERR_FILENO, "Migrane: %s: command not found\n", cmd);
}

static int	handle_child(t_tree *node, t_data *data)
{
	char	*path;

	path = get_absolute_path(node->argv[0], data->env);
	execve(path, node->argv, data->env_vec);
	print_exec_error(node->argv[0]);
	// TODO: garbage collector cleanup here
	free(path);
	free_argv(data->env_vec);
	free_envlist(data->env);
	exit(EXECVE_FAILURE);
}

int	exec_node(t_tree *node, t_data *data)
{
	pid_t	id;
	int		ex_status;

	if (node->fake == true)
		return (EXIT_SUCCESS);
	id = fork(); // protect.
	if (id == 0)
		handle_child(node, data);
	waitpid(id, &ex_status, 0);
	if (WIFEXITED(ex_status))
		return (WEXITSTATUS(ex_status));
	if (WIFSIGNALED(ex_status))
		return (128 + WTERMSIG(ex_status));
	return (ex_status);
}
