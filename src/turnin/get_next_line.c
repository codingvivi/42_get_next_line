/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/30 16:47:39 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

void *ft_memchr(const void *src, int c, size_t count);
void *ft_memcpy(void *dest, const void *src, size_t count);
t_rflags gnl_read(unsigned char **buf, int fd, const unsigned char **r_pos,
                  const unsigned char **r_end);

char *get_next_line(int fd) {
  static t_gnl_buf stream;
  t_gnl_currop curr;
  t_rflags read_result;

  if (!stream.cap) {
    stream = (t_gnl_buf){.cap = MEMSIZE_INIT,
                         .buf = malloc((MEMSIZE_INIT + 1) * sizeof(char))};
    if (!stream.buf)
      return NULL;
  }
  if (stream.read_pos == stream.read_end) {
    read_result =
        gnl_read(&stream.buf, fd, (const unsigned char **)&stream.read_pos,
                 (const unsigned char **)&stream.read_end);
    if (read_result == e_r_err)
      return 0;
    stream.flags |= read_result;
  }
  stream.read_len = stream.read_end - stream.read_pos;
  curr.delim = ft_memchr(stream.read_pos, '\n', stream.read_len);
  if (curr.delim)
    curr.copy_len = curr.delim - stream.read_pos + 1;
  else
    curr.copy_len = stream.read_len;
  curr.outbuf = malloc((curr.copy_len + 1 + 1) * sizeof(char));
  if (!curr.outbuf) {
    free(stream.buf);
    stream.buf = NULL;
    return NULL;
  }

  ft_memcpy(curr.outbuf, (const char *)stream.read_pos, curr.copy_len);
  curr.outbuf[curr.copy_len] = 0;
  stream.read_pos += curr.copy_len;
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
