/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/10 18:54:23 by lrain            ###   ########.fr       */
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
void	gnl_ensure_freed(unsigned char **tgt);

int		ensure_valid_read(t_gnl_buf *s, int fd);
int		gnl_copy(t_gnl_buf *sp, t_gnl_currop *cp);

char	*get_next_line(int fd)
{
	static t_gnl_buf	strm;
	t_gnl_currop		curr;
	unsigned char		*tmp;

	if (!strm.buf)
	{
		strm.buf = malloc((BUFFER_SIZE) * sizeof(char));
		if (!strm.buf)
			return (NULL);
	}
	curr = (t_gnl_currop){};
	tmp = NULL;
	while (1)
	{
		if (ensure_valid_read(&strm, fd))
			break ;
		curr.delim = gnl_memchr('\n', strm.rd_pos, strm.rd_len);
		if (curr.delim)
			curr.copy_len = (curr.delim - strm.rd_pos) + 1;
		else
			curr.copy_len = strm.rd_len;
		if (!curr.outbuf)
		{
			curr.outbuf = malloc((curr.copy_len + 1) * sizeof(char));
			if (!curr.outbuf)
				gnl_ensure_freed(&strm.buf);
			curr.cap = curr.copy_len + 1;
		}
		if (gnl_copy(&strm, &curr))
		{
			gnl_ensure_freed(&strm.buf);
			gnl_ensure_freed(&curr.outbuf);
			return (NULL);
		}
		if (curr.delim)
			break ;
	}
	if (curr.outbuf)
	{
		if (curr.cap < curr.len + 1)
		{
			tmp = scuffed_realloc(curr.len, curr.outbuf, curr.len + 1);
			if (!tmp)
			{
				gnl_ensure_freed(&strm.buf);
				gnl_ensure_freed(&curr.outbuf);
				return (NULL);
			}
			curr.outbuf = tmp;
		}
		curr.outbuf[curr.len++] = 0;
	}
	if (strm.flags)
	{
		gnl_ensure_freed(&strm.buf);
		strm.rd_pos = NULL;
		strm.rd_end = NULL;
		strm.rd_len = 0;
		if (strm.flags & e_gnl_err)
		{
			strm.flags &= ~e_gnl_err;
			gnl_ensure_freed(&curr.outbuf);
		}
		else
			strm.flags = 0;
	}
	return ((char *)curr.outbuf);
}

int	ensure_valid_read(t_gnl_buf *s, int fd)
{
	ssize_t	read_result;

	if (s->rd_pos == s->rd_end)
	{
		read_result = read(fd, s->buf, BUFFER_SIZE);
		if (read_result == e_r_err)
		{
			s->flags |= e_gnl_err;
			return (1);
		}
		if (read_result == e_r_eof)
			s->flags |= e_gnl_eof;
		s->rd_pos = s->buf;
		s->rd_len = (size_t)read_result;
		s->rd_end = s->rd_pos + s->rd_len;
	}
	if (!s->rd_len)
		return (1);
	return (0);
}

int	gnl_copy(t_gnl_buf *sp, t_gnl_currop *cp)
{
	size_t			new_cap;
	unsigned char	*temp;

	if (cp->copy_len + 1 > cp->cap - cp->len)
	{
		new_cap = cp->len + cp->copy_len + 1;
		if (!cp->delim && new_cap < SIZE_MAX / 4)
			new_cap += new_cap / 2;
		temp = scuffed_realloc(cp->len, cp->outbuf, new_cap);
		if (!temp)
		{
			new_cap = cp->cap + cp->copy_len + 1;
			temp = scuffed_realloc(cp->len, cp->outbuf, new_cap);
		}
		if (!temp)
			return (1);
		cp->outbuf = temp;
		cp->cap = new_cap;
	}
	ft_memcpy(cp->outbuf + cp->len, sp->rd_pos, cp->copy_len);
	sp->rd_pos += cp->copy_len;
	sp->rd_len -= cp->copy_len;
	cp->len += cp->copy_len;
	return (0);
}