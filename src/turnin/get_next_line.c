/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/09 19:22:23 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

void	*gnl_memchr(int c, const void *src, size_t count);
void	*ft_memcpy(void *dest, const void *src, size_t count);
void	*free_and_null(t_to_free *tgts);
void	*scuffed_realloc(size_t old_size, void *ptr, size_t new_size);

char	*get_next_line(int fd)
{
	static t_gnl_buf	strm;
	t_gnl_currop		curr;
	ssize_t				read_result;
	size_t				new_cap;
	unsigned char		*tmp;
	static unsigned int	run = 0;

	// /****************************************************************\
	// |                        START init_vars                         |
	// \****************************************************************/
	if (!strm.buf)
	{
		strm.buf = malloc((BUFFER_SIZE) * sizeof(char));
		if (!strm.buf)
			return (NULL);
	}
	curr = (t_gnl_currop){};
	tmp = NULL;
	run++;
	while (1)
	{
		if (strm.rd_pos == strm.rd_end)
		{
			//	/****************************************************************\
			// |                         START gnl_read                         |
			// \****************************************************************/
			if (run == 3)
				read_result = -1;
			else
				read_result = read(fd, strm.buf, BUFFER_SIZE);
			if (read_result == e_r_err)
			{
				strm.flags |= e_gnl_err;
				break ;
			}
			if (read_result == e_r_eof)
				strm.flags |= e_gnl_eof;
			strm.rd_pos = strm.buf;
			strm.rd_len = (size_t)read_result;
			strm.rd_end = strm.rd_pos + strm.rd_len;
		}
		if (!strm.rd_len)
			break ;
		// /****************************************************************\
		// |                        START seek_delim                        |
		// \****************************************************************/
		curr.delim = gnl_memchr('\n', strm.rd_pos, strm.rd_len);
		if (curr.delim)
			curr.copy_len = (curr.delim - strm.rd_pos) + 1;
		else
			curr.copy_len = strm.rd_len;
		// /****************************************************************\
		// |                        START copy_line                         |
		// \****************************************************************/
		if (!curr.outbuf)
		{
			curr.outbuf = malloc((curr.copy_len + 1) * sizeof(char));
			if (!curr.outbuf)
			{
				if (strm.buf)
				{
					free(strm.buf);
					strm.buf = NULL;
				}
			}
			curr.cap = curr.copy_len + 1;
		}
		if (curr.copy_len + 1 > curr.cap - curr.len)
		{
			new_cap = curr.len + curr.copy_len + 1;
			if (!curr.delim && new_cap < SIZE_MAX / 4)
				new_cap += new_cap / 2;
			tmp = scuffed_realloc(curr.len, curr.outbuf, new_cap);
			if (!tmp)
			{
				new_cap = curr.cap + curr.copy_len + 1;
				tmp = scuffed_realloc(curr.len, curr.outbuf, new_cap);
			}
			if (!tmp)
			{
				if (strm.buf)
				{
					free(strm.buf);
					strm.buf = NULL;
				}
				if (curr.outbuf)
				{
					free(curr.outbuf);
					curr.outbuf = NULL;
				}
				return (NULL);
			}
			curr.outbuf = tmp;
			curr.cap = new_cap;
		}
		ft_memcpy(curr.outbuf + curr.len, strm.rd_pos, curr.copy_len);
		strm.rd_pos += curr.copy_len;
		strm.rd_len -= curr.copy_len;
		curr.len += curr.copy_len;
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
				if (strm.buf)
				{
					free(strm.buf);
					strm.buf = NULL;
				}
				if (curr.outbuf)
				{
					free(curr.outbuf);
					curr.outbuf = NULL;
				}
				return (NULL);
			}
			curr.outbuf = tmp;
		}
		curr.outbuf[curr.len++] = 0;
	}
	if (strm.flags & e_gnl_err)
	{
		free(strm.buf);
		strm.buf = NULL;
		strm.rd_pos = NULL;
		strm.rd_end = NULL;
		strm.rd_len = 0;
		strm.flags &= ~e_gnl_err;
	}
	if (strm.flags & e_gnl_eof)
	{
		free(strm.buf);
		strm.buf = NULL;
		strm.rd_pos = NULL;
		strm.rd_end = NULL;
		strm.rd_len = 0;
		strm.flags = 0;
	}
	return ((char *)curr.outbuf);
}