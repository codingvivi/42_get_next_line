/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/31 23:45:47 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gnl_read(int fd, t_gnl_buf *s);
void *mem_chr_or_cpy(const void *src, void *dest_cpy, size_t count, int c_chr);

char *get_next_line(int fd) {
  static t_gnl_buf strm = (t_gnl_buf){.cap = MEMSIZE_INIT};
  t_gnl_currop curr;

  if (!strm.buf) {
    strm = (t_gnl_buf){.buf = malloc((MEMSIZE_INIT + 1) * sizeof(char))};
    if (!strm.buf)
      return NULL;
  }

  if (!strm.rd_len) {
    if (gnl_read(fd, &strm))
      return NULL;
  }

  curr.delim = mem_chr_or_cpy(strm.rd_pos, DO_MEMCHR, strm.rd_len, '\n');
  if (curr.delim)
    curr.copy_len = curr.delim - strm.rd_pos + 1;
  else
    curr.copy_len = strm.rd_len;
  curr.outbuf = malloc((curr.copy_len + 1 + 1) * sizeof(char));
  if (!curr.outbuf) {
    free(strm.buf);
    strm.buf = NULL;
    return NULL;
  }

  mem_chr_or_cpy((const char *)strm.rd_pos, curr.outbuf, curr.copy_len, NOCHR);
  curr.outbuf[curr.copy_len] = 0;
  strm.rd_pos += curr.copy_len;
  strm.rd_len -= curr.copy_len;
  return curr.outbuf;
}
/*
char *get_next_line(int fd) {

  static t_gnl_buf stream;
  unsigned char *delim;
  char *temp;
  if (!stream.len) {
    stream = (t_gnl_buf){.capacity = rev_clamp(BUFSIZ, BUFFER_SIZE),
                         .buf = malloc((stream.capacity + 1) * sizeof(char))};
  }
  if (!stream.buf)
    read(fd, stream.buf, BUFFER_SIZE);
  if (!stream.buf) {
  }
  return NULL;
}
*/
