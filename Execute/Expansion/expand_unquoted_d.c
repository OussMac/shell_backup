#include "../execute.h"

int expand_unqoted_d(char ***pockets, t_data *data, char *raw)
{
    int     i;
    int     j;
    char    **new_pocket;
    int     mc_argc;
    char    **split;

    split = ft_split(raw, ' ');
    mc_argc = arg_count(split);
    new_pocket = malloc((data->pc.cap + mc_argc + 1) * sizeof(char *));
    i = 0;
    while (i < data->pc.j)
    {
        new_pocket[i] = ft_strdup((*pockets)[i]);
        i++;
    }
    j = 0;
    while (split[j])
    {
        new_pocket[i++] = append_delimiter(split[j++]);
    }
    new_pocket[i] = NULL;
    (*pockets) = new_pocket;
    data->pc.j += i; // update j to the new size.
    return (EXIT_SUCCESS);
}
