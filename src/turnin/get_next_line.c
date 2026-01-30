/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/30 17:55:08 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

void *ft_memchr(const void *src, int c, size_t count);
void *ft_memcpy(void *dest, const void *src, size_t count);
void *mem_chr_or_cpy(const void *src, void *dest_cpy, size_t count, int c_chr);
t_rflags gnl_read(unsigned char **buf, int fd, const unsigned char **r_pos,
                  const unsigned char **r_end);

char *get_next_line(int fd) {
  static t_gnl_buf strm;
  t_gnl_currop curr;
  t_rflags read_result;

  if (!strm.cap) {
    strm = (t_gnl_buf){.cap = MEMSIZE_INIT,
                       .buf = malloc((MEMSIZE_INIT + 1) * sizeof(char))};
    if (!strm.buf)
      return NULL;
  }
  if (strm.read_pos == strm.read_end) {
    read_result =
        gnl_read(&strm.buf, fd, (const unsigned char **)&strm.read_pos,
                 (const unsigned char **)&strm.read_end);
    if (read_result == e_r_err)
      return 0;
    strm.flags |= read_result;
  }
  strm.read_len = strm.read_end - strm.read_pos;
  curr.delim = mem_chr_or_cpy(strm.read_pos, DO_MEMCHR, strm.read_len, '\n');
  if (curr.delim)
    curr.copy_len = curr.delim - strm.read_pos + 1;
  else
    curr.copy_len = strm.read_len;
  curr.outbuf = malloc((curr.copy_len + 1 + 1) * sizeof(char));
  if (!curr.outbuf) {
    free(strm.buf);
    strm.buf = NULL;
    return NULL;
  }

  mem_chr_or_cpy((const char *)strm.read_pos, curr.outbuf, curr.copy_len,
                 NO_SEEK);
  curr.outbuf[curr.copy_len] = 0;
  strm.read_pos += curr.copy_len;
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
