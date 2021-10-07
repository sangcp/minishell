#include "minishell.h"

int is_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}

void path_free(char **str)
{
	int i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}