/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/10 23:09:38 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	*gnl_memchr(int c, const void *src, size_t count);
void	*ft_memcpy(void *dest, const void *src, size_t count);
void	*scuffed_realloc(size_t old_size, void *ptr, size_t new_size);
void	ensure_freed(unsigned char **tgt);

int		gnl_get_line(t_gnl_currop *cp, int fd, t_gnl_buf *sp);
int		ensure_valid_read(t_gnl_buf **spp, int fd);
int		copy_found(t_gnl_buf **sp, t_gnl_currop **cp);
char	*get_output_val(t_gnl_currop *cp, t_gnl_buf *sp);

char	*get_next_line(int fd)
{
	static t_gnl_buf	strm;
	t_gnl_currop		curr;

	if (!strm.buf)
	{
		strm.buf = malloc((BUFFER_SIZE) * sizeof(char));
		if (!strm.buf)
			return (NULL);
	}
	curr = (t_gnl_currop){};
	if (gnl_get_line(&curr, fd, &strm))
		return (NULL);
	return (get_output_val(&curr, &strm));
}

int	gnl_get_line(t_gnl_currop *cp, int fd, t_gnl_buf *sp)
{
	while (1)
	{
		if (ensure_valid_read(&sp, fd))
			break ;
		cp->delim = gnl_memchr('\n', sp->rd_pos, sp->rd_len);
		if (cp->delim)
			cp->copy_len = (cp->delim - sp->rd_pos) + 1;
		else
			cp->copy_len = sp->rd_len;
		if (!cp->outbuf)
		{
			cp->outbuf = malloc((cp->copy_len + 1) * sizeof(char));
			if (!cp->outbuf)
				ensure_freed(&sp->buf);
			cp->cap = cp->copy_len + 1;
		}
		cp->temp = NULL;
		if (copy_found(&sp, &cp))
		{
			ensure_freed(&sp->buf);
			ensure_freed(&cp->outbuf);
			return (1);
		}
		if (cp->delim)
			break ;
	}
	return (0);
}

int	ensure_valid_read(t_gnl_buf **spp, int fd)
{
	ssize_t		read_result;
	t_gnl_buf	*sp;

	sp = *spp;
	if (sp->rd_pos == sp->rd_end)
	{
		read_result = read(fd, sp->buf, BUFFER_SIZE);
		if (read_result == e_r_err)
		{
			sp->flags |= e_gnl_err;
			return (1);
		}
		if (read_result == e_r_eof)
			sp->flags |= e_gnl_eof;
		sp->rd_pos = sp->buf;
		sp->rd_len = (size_t)read_result;
		sp->rd_end = sp->rd_pos + sp->rd_len;
	}
	if (!sp->rd_len)
		return (1);
	return (0);
}

/* grows buffer to exact size if delimiter found,
or geometrically, if not found,
copies result */
int	copy_found(t_gnl_buf **spp, t_gnl_currop **cpp)
{
	t_gnl_currop	*cp;
	t_gnl_buf		*sp;

	cp = *cpp;
	sp = *spp;
	if (cp->copy_len + 1 > cp->cap - cp->len)
	{
		cp->newcap = cp->len + cp->copy_len + 1;
		if (!cp->delim && cp->newcap < SIZE_MAX / 4)
			cp->newcap += cp->newcap / 2;
		cp->temp = scuffed_realloc(cp->len, cp->outbuf, cp->newcap);
		if (!cp->temp)
		{
			cp->newcap = cp->cap + cp->copy_len + 1;
			cp->temp = scuffed_realloc(cp->len, cp->outbuf, cp->newcap);
		}
		if (!cp->temp)
			return (1);
		cp->outbuf = cp->temp;
		cp->cap = cp->newcap;
	}
	ft_memcpy(cp->outbuf + cp->len, sp->rd_pos, cp->copy_len);
	sp->rd_pos += cp->copy_len;
	sp->rd_len -= cp->copy_len;
	cp->len += cp->copy_len;
	return (0);
}

char	*get_output_val(t_gnl_currop *cp, t_gnl_buf *sp)
{
	unsigned char	*tmp;
	unsigned int	eof_state;

	if (cp->outbuf)
	{
		if (cp->cap < cp->len + 1)
		{
			tmp = scuffed_realloc(cp->len, cp->outbuf, cp->len + 1);
			if (!tmp)
			{
				ensure_freed(&sp->buf);
				ensure_freed(&cp->outbuf);
				return (NULL);
			}
			cp->outbuf = tmp;
		}
		cp->outbuf[cp->len++] = 0;
	}
	if (sp->flags)
	{
		eof_state = 0;
		ensure_freed(&sp->buf);
		if (sp->flags & e_gnl_err)
		{
			sp->flags &= ~e_gnl_err;
			eof_state = sp->flags;
			ensure_freed(&cp->outbuf);
		}
		*sp = (t_gnl_buf){};
		sp->flags = eof_state;
	}
	return ((char *)cp->outbuf);
}