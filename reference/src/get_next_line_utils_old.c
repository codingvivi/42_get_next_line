/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/02/02 17:38:19 by lrain            ###   ########.fr       */
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
int gnl_read(int fd, t_gnl_buf *s) {
  ssize_t read_result;

  read_result = read(fd, s->buf, BUFFER_SIZE);
  if (read_result < e_r_normal_bounds)
    return 1;
  if (read_result != BUFFER_SIZE)
    s->flags |= e_r_eof;
  s->rd_pos = s->buf;
  s->rd_len = (size_t)read_result;
  s->rd_end = s->rd_pos + s->rd_len;
  return 0;
}
/* truly deranged */
void *mem_chr_or_cpy(const void *src, void *dest_cpy, size_t count, int c_chr) {
  unsigned char *d;
  void *out;
  const unsigned char *s = src;

  if (dest_cpy == DO_MEMCHR) {
    c_chr = (unsigned char)c_chr;
    while (count && (*s != c_chr)) {
      s++;
      count--;
    }
    if (count)
      out = ((void *)s);
    else
      out = NULL;
  } else {
    out = dest_cpy;
    if (!dest_cpy && !src && count)
      return (NULL);
    d = dest_cpy;
    while (count--)
      *d++ = *s++;
  }
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
