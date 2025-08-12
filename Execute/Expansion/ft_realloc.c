#include "../execute.h"

char    **realloc_pockets(char **old_pocket, size_t old_cap, size_t new_cap)
{
    size_t  i;
    char    **new_pocket;

    new_pocket = malloc (sizeof(char *) * (new_cap + 1));
    if (!new_pocket)
        return (NULL);
    i = 0;
    while (i < old_cap)
    {
        if (old_pocket[i])
            new_pocket[i] = old_pocket[i];
        else
            new_pocket[i] = NULL;
        i++;
    }
    // free(old_pocket);
    return (new_pocket);
}


// possibly almost there why segfaults in strjoin


// // Safe realloc implementation
// void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
// {
//     void *new_ptr = malloc(new_size);
//     if (!new_ptr)
//         return (NULL);
    
//     if (ptr)
//     {
//         memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
//         free(ptr);
//     }
//     return (new_ptr);
// }