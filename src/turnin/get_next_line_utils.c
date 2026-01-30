/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/01/26 22:03:39 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define READ_ERR -1
#define READ_NONE_READ 0
#define READ_EOF READ_NONE_READ

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

t_rflags gnl_read(unsigned char **buf, int fd, const unsigned char **r_pos,
                  const unsigned char **r_end) {
  ssize_t init_read_len;
  t_rflags out;

  init_read_len = read(fd, *buf, BUFFER_SIZE);
  if (init_read_len == READ_ERR || init_read_len == READ_EOF)
    return e_r_err;
  if (!*r_pos)
    *r_pos = *buf;
  *r_end = *r_pos + (size_t)init_read_len;
  if (init_read_len != BUFFER_SIZE)
    out = e_r_eof;
  else
    out = e_r_normal;
  return out;
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
