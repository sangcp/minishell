#include "minishell.h"

int find_equal(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '=')
            return (1);
        i++;
    }
    return (0);
}

int cmd_env(char **args, char **envp)
{
    int i;

    i = 0;
    if (!(ft_strcmp(args[0],"env")))
    {
        while (envp[i])
        {
            printf("%s\n", envp[i]);
            i++;
        }
    }
    else
    {
        while (envp[i])
        {
            printf("%s\n", envp[i]);
            i++;
        }
        if (find_equal(args[1]))
            printf("%s\n",args[1]);
        else
            printf("env : %s: No such file or directory\n", args[1]);
    }
    return (1);
}