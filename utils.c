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

void free_all(t_shell *mini, t_list *list)
{
	t_list *tlist;
	int i = 0;

	(void)mini;
	if (list == NULL)
		return ;
	tlist = list;
	while (tlist->next)
	{
		while (((t_ops *)(tlist->content))->args[i])
		{
			free(((t_ops *)(tlist->content))->args[i]);
			i++;
		}
		free(((t_ops *)(tlist->content))->operation);
		free(((t_ops *)(tlist->content))->args);
		free(((t_ops *)(tlist->content)));
		free(tlist);
		tlist = tlist->next;
		i = 0;
	}
	while (((t_ops *)(tlist->content))->args[i])
	{
		free(((t_ops *)(tlist->content))->args[i]);
		i++;
	}
	if (((t_ops *)(tlist->content))->operation)
		free(((t_ops *)(tlist->content))->operation);
	if (((t_ops *)(tlist->content))->args)
		free(((t_ops *)(tlist->content))->args);
	if (((t_ops *)(tlist->content)))
		free(((t_ops *)(tlist->content)));
	if (tlist)
		free(tlist);
}
