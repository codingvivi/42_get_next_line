/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/13 15:42:46 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void			*gnl_memchr(int c, const void *src, size_t count);
void			*ft_memcpy(void *dest, const void *src, size_t count);
void			*scuffed_realloc(size_t old_size, void *ptr, size_t new_size);
void			ensure_freed(unsigned char **tgt);

t_gnl_fflags	line_fetch(t_gnl_currop *cp, int fd, t_gnl_buf *sp);
bool			ensure_buf_to_seek(t_gnl_buf **spp, int fd);
bool			copy_found(t_gnl_buf *sp, t_gnl_currop *cp);
char			*get_output_val(t_gnl_currop *cp, t_gnl_buf *sp);
void			*gnl_freeall_b4_null(unsigned char **cb_pp,
					unsigned char **sb_pp);

char	*get_next_line(int fd)
{
	static t_gnl_buf	strm;
	t_gnl_currop		curr;
	t_gnl_fflags		fetchop_res;

	if (!strm.buf)
	{
		strm.buf = malloc((BUFFER_SIZE) * sizeof(char));
		if (!strm.buf)
			return (NULL);
	}
	curr = (t_gnl_currop){};
	while (1)
	{
		fetchop_res = line_fetch(&curr, fd, &strm);
		if (fetchop_res == e_gnl_abort)
			return (gnl_freeall_b4_null(&curr.outbuf, &strm.buf));
		if (fetchop_res == e_gnl_break)
			break ;
	}
	return (get_output_val(&curr, &strm));
}

t_gnl_fflags	line_fetch(t_gnl_currop *cp, int fd, t_gnl_buf *sp)
{
	if (!ensure_buf_to_seek(&sp, fd))
		return (e_gnl_break);
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
	if (!copy_found(sp, cp))
		return (e_gnl_abort);
	if (cp->delim)
		return (e_gnl_break);
	return (e_gnl_continue);
}

bool	ensure_buf_to_seek(t_gnl_buf **spp, int fd)
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
			return (false);
		}
		if (read_result == e_r_eof)
			sp->flags |= e_gnl_eof;
		sp->rd_pos = sp->buf;
		sp->rd_len = (size_t)read_result;
		sp->rd_end = sp->rd_pos + sp->rd_len;
	}
	if (!sp->rd_len)
		return (false);
	return (true);
}

/* grows buffer to exact size if delimiter found,
or geometrically, if not found,
copies result */
bool	copy_found(t_gnl_buf *sp, t_gnl_currop *cp)
{
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
			return (false);
		cp->outbuf = cp->temp;
		cp->temp = NULL;
		cp->cap = cp->newcap;
	}
	ft_memcpy(cp->outbuf + cp->len, sp->rd_pos, cp->copy_len);
	sp->rd_pos += cp->copy_len;
	sp->rd_len -= cp->copy_len;
	cp->len += cp->copy_len;
	return (true);
}

char	*get_output_val(t_gnl_currop *cp, t_gnl_buf *sp)
{
	if (cp->outbuf)
	{
		if (cp->cap < cp->len + 1)
		{
			cp->temp = scuffed_realloc(cp->len, cp->outbuf, cp->len + 1);
			if (!cp->temp)
				return (gnl_freeall_b4_null(&cp->outbuf, &sp->buf));
			cp->outbuf = cp->temp;
		}
		cp->outbuf[cp->len++] = 0;
	}
	if (sp->flags)
	{
		ensure_freed(&sp->buf);
		if (sp->flags & e_gnl_err)
		{
			sp->flags &= ~e_gnl_err;
			cp->eof_state = sp->flags;
			ensure_freed(&cp->outbuf);
		}
		*sp = (t_gnl_buf){};
		sp->flags = cp->eof_state;
	}
	return ((char *)cp->outbuf);
}
