/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/06 15:10:14 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define OUTBUF_INDEX 1

void	*free_and_null(t_to_free *tgts);
void	*scuffed_realloc(size_t old_size, void *ptr, size_t new_size);
void	*gnl_memchr(int c, const void *src, size_t count);
void	*ft_memcpy(void *dest, const void *src, size_t count);
int		gnl_read(int fd, t_gnl_buf *sp);
int		seek_delim(unsigned char **dlm, size_t *cpy_len, t_gnl_buf *sp);
int		copy_line(unsigned char **read_pos, t_gnl_currop *curr_p,
			unsigned char **buf, size_t *buf_len, t_to_free *itf_p);
int		init_vars(unsigned char **sb_p, t_to_free *itf_p,
			unsigned char **obuf_p, size_t *cap_p);

char	*get_next_line(int fd)
{
	static t_gnl_buf	strm;
	t_to_free			items2f;
	t_gnl_currop		curr;
	unsigned char		*temp;

	items2f = (t_to_free){};
	curr = (t_gnl_currop){};
	if (init_vars(&strm.buf, &items2f, &curr.outbuf, &curr.cap))
		return (free_and_null(&items2f));
	while (1)
	{
		if (strm.rd_pos == strm.rd_end)
		{
			if (gnl_read(fd, &strm))
				return (free_and_null(&items2f));
		}
		if (seek_delim(&curr.delim, &curr.copy_len, &strm))
			return (free_and_null(&items2f));
		if (copy_line(&strm.rd_pos, &curr, &curr.outbuf, &curr.len, &items2f))
			return (free_and_null(&items2f));
		if (curr.delim)
			break ;
	}
	curr.outbuf[curr.len++] = 0;
	if (curr.cap > curr.len)
	{
		temp = scuffed_realloc(curr.len, curr.outbuf, curr.len);
		if (!temp)
			return (free_and_null(&items2f));
		curr.outbuf = temp;
	}
	return ((char *)curr.outbuf);
}

int	init_vars(unsigned char **sb_p, t_to_free *itf_p, unsigned char **obuf_p,
		size_t *cap_p)
{
	if (!*sb_p)
	{
		*sb_p = malloc((BUFSIZ + 1) * sizeof(char));
		itf_p->ptrs[itf_p->num++] = *sb_p;
		if (!sb_p)
			return (1);
	}
	*obuf_p = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!*obuf_p)
		return (1);
	*cap_p = BUFFER_SIZE;
	itf_p->ptrs[itf_p->num++] = *obuf_p;
	return (0);
}

int	gnl_read(int fd, t_gnl_buf *sp)
{
	const ssize_t	read_result = read(fd, sp->buf, BUFFER_SIZE);

	if (read_result < e_r_normal_bounds)
		return (1);
	if (read_result != BUFFER_SIZE)
		sp->flags |= e_r_eof;
	sp->rd_pos = sp->buf;
	sp->rd_len = (size_t)read_result;
	sp->rd_end = sp->rd_pos + sp->rd_len;
	return (0);
}

int	seek_delim(unsigned char **dlm, size_t *cpy_len, t_gnl_buf *sp)
{
	*dlm = gnl_memchr('\n', sp->rd_pos, sp->rd_len);
	if (*dlm)
		*cpy_len = (*dlm - sp->rd_pos) + 1;
	else
		*cpy_len = sp->rd_len;
	return (0);
}

/*
  I still know it's unhinged to pass struct AND individual members,
  but this is leagues more readable than 18 struct member calls.
  I wouldn't do this in projects outside of 42 ever, I promise.
*/
int	copy_line(unsigned char **read_pos, t_gnl_currop *curr_p,
		unsigned char **buf, size_t *buf_len, t_to_free *itf_p)
{
	size_t				cap;
	size_t				copy_len;
	const unsigned char	*delim = curr_p->delim;
	size_t				new_cap;
	unsigned char		*tmp;

	cap = curr_p->cap;
	copy_len = curr_p->copy_len;
	if (copy_len + 1 > cap - *buf_len)
	{
		new_cap = *buf_len + copy_len + 1;
		if (!delim && new_cap < SIZE_MAX / 4)
			new_cap += new_cap / 2;
		tmp = scuffed_realloc(*buf_len, *buf, new_cap);
		if (!tmp)
			tmp = scuffed_realloc(*buf_len, *buf, cap + copy_len + 1);
		if (!tmp)
			return (1);
		*buf = tmp;
		itf_p->ptrs[OUTBUF_INDEX] = *buf;
		cap = new_cap;
	}
	if (copy_len)
	{
		ft_memcpy(*buf + *buf_len, *read_pos, copy_len);
		*read_pos += copy_len;
		*buf_len += copy_len;
	}
	return (0);
}