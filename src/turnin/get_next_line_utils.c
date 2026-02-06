/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/02/06 17:11:58 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_ERR -1
#define READ_NONE_READ 0
#define READ_EOF READ_NONE_READ

void	*gnl_memchr(int c, const void *src, size_t count)
{
	const unsigned char	*s = src;
	unsigned char		uc;

	uc = (unsigned char)c;
	while (count--)
	{
		if (*s == uc)
			return ((void *)s);
		s++;
	}
	return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t count)
{
	unsigned char		*d;
	const unsigned char	*s = src;

	if (!dest && !src && count)
		return (NULL);
	d = dest;
	while (count--)
		*d++ = *s++;
	return (dest);
}

void	*free_and_null(t_to_free *tgts)
{
	void	**curr;

	curr = &tgts->ptrs[0];
	if (!tgts || !tgts->ptrs)
		return (NULL);
	while (tgts->num)
	{
		if (*curr)
			free(*curr);
		*curr = NULL;
		curr++;
		tgts->num--;
	}
	free((void *)tgts->ptrs);
	tgts->ptrs = NULL;
	return (NULL);
}

void	*scuffed_realloc(size_t old_size, void *ptr, size_t new_size)
{
	void	*new_ptr;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, old_size);
		free(ptr);
	}
	return (new_ptr);
}

/* char *gnl_setcap(t_gnl_buf stream, t_gnl_currop curr) {
   unsigned char *tmp;
   if (!curr.delim && curr.newcap < SIZE_MAX / 4) {
   }
 }

 int gnl_read(t_gnl_buf stream, const int fd) {
   if () {
   }
   stream.read_len = read(fd, stream.buf, BUFFER_SIZE);
   if (stream.read_len == READ_ERR)
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																return (-1);
 } */