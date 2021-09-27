#include "minishell.h"

int is_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}
