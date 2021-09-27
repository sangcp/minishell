#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include "libft/libft.h"

int is_quotes(char c);
void path_free(char **str);

#endif