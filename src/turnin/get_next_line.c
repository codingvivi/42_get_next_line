/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/21 21:18:11 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

#define READ_EOF READ_NONE_READ

void *ft_memchr(const void *src, int c, size_t count);
void *ft_memcpy(void *dest, const void *src, size_t count);

char *get_next_line(int fd) {
  static t_gnl_buf stream;
  t_gnl_currop curr;

  // init vars
  if (BUFFER_SIZE <= 0)
    return NULL;
  if (!stream.cap) {
    stream = (t_gnl_buf){.cap = MEMSIZE_INIT,
                         .buf = malloc((MEMSIZE_INIT + 1) * sizeof(char))};
  }
  if (!stream.buf)
    return NULL;

  stream.read_len = read(fd, stream.buf, BUFFER_SIZE);
  if (stream.read_len == READ_ERR || stream.read_len == READ_EOF)
    return NULL;
  if (stream.read_len != BUFFER_SIZE)
    stream.flags |= STREAM_EOF;
  if (!stream.read_pos)
    stream.read_pos = stream.buf;
  curr.delim = ft_memchr(stream.buf, '\n', BUFFER_SIZE);
  if (curr.delim)
    curr.copy_len = curr.delim - stream.read_pos;
  else
    curr.copy_len = stream.read_len;

  curr.outbuf = malloc((curr.copy_len + 1) * sizeof(char));
  if (!curr.outbuf) {
    free(stream.buf);
    stream.buf = NULL;
    return NULL;
  }
  ft_memcpy(curr.outbuf, (const char *)stream.buf, curr.copy_len);
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
