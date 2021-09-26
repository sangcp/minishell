/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangcpar <sangcpar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 21:04:45 by sangcpar          #+#    #+#             */
/*   Updated: 2020/11/04 14:10:45 by sangcpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

int			get_next_line(const int fd, char **line)
{
	static char	*cpy[10000];
	char		buf[BUFF_SIZE + 1];
	char		*tmp;
	int			rd_ret;

	if (fd < 0 || !line || !(cpy[fd] = !cpy[fd] ? ft_strnew(0) : cpy[fd]) ||
			(read(fd, buf, 0) < 0) || fd >= 10000)
		return (-1);
	while ((rd_ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[rd_ret] = 0;
		tmp = ft_strjoin(cpy[fd], buf);
		free(cpy[fd]);
		cpy[fd] = tmp;
		if (ft_strchr(buf, ENDL))
			break ;
	}
	if (rd_ret < 0)
		return (-1);
	else if (!rd_ret && (!cpy[fd] || !cpy[fd][0]))
	{
		free(cpy[fd]);
		return (0);
	}
	return (ft_cleanup(cpy, line, fd, rd_ret));
}
