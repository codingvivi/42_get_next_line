/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/01/21 21:16:27 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

void *ft_memchr(const void *src, int c, size_t count) {
  void *out;
  const unsigned char *s = src;

  c = (unsigned char)c;
  while (count && (*s != c)) {
    s++;
    count--;
  }
  if (count)
    out = ((void *)s);
  else
    out = NULL;
  return out;
}

void *ft_memcpy(void *dest, const void *src, size_t count) {
  unsigned char *d;
  const unsigned char *s = src;

  if (!dest && !src && count)
    return (NULL);
  d = dest;
  while (count--)
    *d++ = *s++;
  return (dest);
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
    return -1;
} */
