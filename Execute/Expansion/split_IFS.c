#include "../execute.h"

bool    has_delim(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == (char)27)
            return (true);
        i++;
    }
    return (false);
}

bool    has_space(char *str)
{
    int i;

    i = 0;
    if (!str)
    {
        printf("null str\n");
        exit(1);
    }
    while (str[i])
    {
        if (str[i] == ' ')
            return (true);
        i++;
    }
    return (false);
}

static  char *append_delimiter(char *str)
{
    char    *first_border;
    char    *last_border;
    char    delim[2];

    delim[0] = (char)27;
    delim[1] = '\0';

    first_border= ft_strjoin(delim, str);
    if (!first_border)
        return (NULL);
    last_border = ft_strjoin(first_border, delim);
    if (!last_border)
        return (free(first_border), NULL);
    free(first_border);
    return (last_border);
}

void free_ifs_list(t_ifs *ifs)
{
    t_ifs   *tmp;

    if (!ifs)
        return ;
    while (ifs)
    {
        tmp = ifs->next;
        free(ifs->string);
        free(ifs);
        ifs = tmp;
    }
}

bool only_spaces(char *raw)
{
    int i;

    i = 0;
    while (raw[i])
    {
        if (raw[i] != ' ')
            return (false);
        i++;
    }
    return (true);
}

int internal_field_seperator(char *raw, t_data *data, char ***pockets)
{
    char    **mini_pocket;
    char    **new_pocket;
    size_t  mc_argc;
    size_t  i;

    mini_pocket = ft_split(raw, ' '); // might have to add tabs and \n
    if (!mini_pocket)
        return (EXIT_FAILURE);
    mc_argc = arg_count(mini_pocket); // a b c d ==> 4
    if (data->pc.j + mc_argc >= data->pc.cap)
    {
        new_pocket = realloc_pockets(*pockets, data->pc.j, data->pc.j + mc_argc);
        if (!new_pocket)
            return (free_argv(mini_pocket), EXIT_FAILURE);
        *pockets = new_pocket;
        data->pc.cap = data->pc.j + mc_argc;
    }
    print_argv(mini_pocket);
    i = 0;
    while (i < mc_argc)
    {
        (*pockets)[data->pc.j] = append_delimiter(mini_pocket[i++]);
        if (!(*pockets)[data->pc.j])
            return (free_argv(mini_pocket), EXIT_FAILURE);
        data->pc.j++;
    }
    return (free_argv(mini_pocket), EXIT_SUCCESS);
}


// Helper to merge pocket arrays
// int merge_pockets(char ***main_pockets, char **new_pockets, t_data *data)
// {
//     size_t new_count = arg_count(new_pockets);
//     size_t total = data->pc.j + new_count;
//     size_t i = 0;
    
//     // Simple realloc without manual capacity tracking
//     *main_pockets = ft_realloc(*main_pockets,
//         (data->pc.j + 1) * sizeof(char *),
//         (total + 1) * sizeof(char *));
//     if (!*main_pockets)
//         return (EXIT_FAILURE);
    
//     // Copy new pockets
//     while (i < new_count)
//     {
//         (*main_pockets)[data->pc.j + i] = new_pockets[i];
//         i++;
//     }
    
//     data->pc.j += new_count;
//     (*main_pockets)[data->pc.j] = NULL;
//     return (EXIT_SUCCESS);
// }


// // Simplified internal_field_seperator
// int internal_field_seperator(char *raw, t_data *data, char ***pockets)
// {
//     char **mini_pocket = ft_split(raw, ' ');
//     size_t i = 0;
//     char *temp;

//     if (!mini_pocket)
//         return (EXIT_FAILURE);
    
//     while (mini_pocket[i])
//     {
//         // Append delimiter directly without intermediate allocations
//         temp = append_delimiter(mini_pocket[i]);
//         if (!temp)
//         {
//             free_argv(mini_pocket);
//             return (EXIT_FAILURE);
//         }
        
//         // Replace current pocket with new value
//         free(mini_pocket[i]);
//         mini_pocket[i] = temp;
//         i++;
//     }
    
//     // Merge mini_pocket into main pockets array
//     if (merge_pockets(pockets, mini_pocket, data) != EXIT_SUCCESS)
//     {
//         free_argv(mini_pocket);
//         return (EXIT_FAILURE);
//     }
    
//     free_argv(mini_pocket);
//     return (EXIT_SUCCESS);
// }


// int internal_field_seperator(char *raw, t_data *data, char ***pockets)
// {
//     char    **mini_pocket;
//     char    **new_pocket;
//     t_ifs   *args;
//     size_t  mc_argc;
//     size_t  i;

//     mini_pocket = ft_split(raw, ' '); // might have to add tabs and \n
//     if (!mini_pocket)
//         return (EXIT_FAILURE);
//     mc_argc = arg_count(mini_pocket);
//     new_pocket = malloc (sizeof(char *) * (data->pc.cap + mc_argc));
//     if (!new_pocket)
//         return (EXIT_FAILURE); // clean
//     i = 0;
//     printf("==========> j: %d\n", data->pc.j);
//     while(i < data->pc.j)
//     {
//         new_pocket[i] = ft_strdup((*pockets)[i]);
//         i++;
//     }
//     int j = 0;
//     while(mini_pocket[j])
//     {
//         new_pocket[i] = append_delimiter(mini_pocket[j]);
//         if (!new_pocket[i])
//             return (EXIT_FAILURE); //clean
//         i++;
//         j++;
//     }
//     //export x="a b" && echo $x
//     // free(*pockets);
//     // free(mini_pocket);
//     *pockets = new_pocket;
//     printf ("=======> j [%d] = allocated cap %zu + mini pocket %zu\n", data->pc.j, data->pc.cap, mc_argc);
//     data->pc.j += mc_argc; // print these values and debug.
//     data->pc.cap = data->pc.cap + mc_argc;
//     printf ("=======> j [%d] = allocated cap %zu + mini pocket %zu\n", data->pc.j, data->pc.cap, mc_argc);
//     return (EXIT_SUCCESS);
// }
