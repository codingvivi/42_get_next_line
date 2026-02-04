/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/04 01:17:51 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void free_all(t_to_free *tgts);
void *scuffed_realloc(size_t old_size, void *ptr, size_t new_size);
int get_delim(int fd, t_gnl_buf *sp, size_t *cpy_len, const unsigned char *dlm);
void *gnl_memchr(int c, const void *src, size_t count);
int copy_line(unsigned char **read_pos, const t_gnl_currop *currop,
              unsigned char **buf, size_t *buf_len);
void *ft_memcpy(void *dest, const void *src, size_t count);

char *get_next_line(int fd) {
  static t_gnl_buf strm;
  t_to_free items2f = (t_to_free){};
  t_gnl_currop curr = (t_gnl_currop){
      .outbuf = malloc((BUFFER_SIZE + 1) * sizeof(char)),
      .cap = BUFFER_SIZE,
  };

  if (!curr.outbuf)
    return NULL;
  items2f.ptrs[items2f.num++] = curr.outbuf;
  if (!strm.buf) {
    strm.buf = malloc((BUFSIZ + 1) * sizeof(char));
    items2f.ptrs[items2f.num++] = strm.buf;
    if (!strm.buf)
      free_all(&items2f);
    return NULL;
  }
  while (1) {
    if (strm.rd_pos == strm.rd_end) {
      if (get_delim(fd, &strm, &curr.copy_len, curr.delim)) {
        free_all(&items2f);
        return NULL;
      }
    }
    if (copy_line(&strm.rd_pos, &curr, &curr.outbuf, &curr.cap)) {
      free_all(&items2f);
      return NULL;
    };
    if (curr.delim)
      break;
  }
  curr.outbuf[curr.len] = 0;
  return ((char *)curr.outbuf);
}

int get_delim(int fd, t_gnl_buf *sp, size_t *cpy_len,
              const unsigned char *dlm) {
  ssize_t read_result;

  read_result = read(fd, sp->buf, BUFFER_SIZE);
  if (read_result < e_r_normal_bounds)
    return 1;
  if (read_result != BUFFER_SIZE)
    sp->flags |= e_r_eof;
  sp->rd_pos = sp->buf;
  sp->rd_len = (size_t)read_result;
  sp->rd_end = sp->rd_pos + sp->rd_len;
  dlm = gnl_memchr('\n', sp->buf, sp->rd_len);
  if (dlm)
    *cpy_len = dlm - sp->rd_pos + 1;
  else
    *cpy_len = sp->rd_len;
  return 0;
}

/*
  Used this opportunity to make the mutables separate args,
  and the struct const.
  I know it's still unhinged to pass both,
  but this is leagues more readable than 18 struct member calls.
  I would do this in projects outside of 42 ever, I promise.
*/

int copy_line(unsigned char **read_pos, const t_gnl_currop *currop,
              unsigned char **buf, size_t *buf_len) {
  const size_t cap = currop->cap;
  const size_t copy_len = currop->copy_len;
  const unsigned char *delim = currop->delim;
  size_t new_cap;
  unsigned char *tmp;

  if (copy_len + 1 > cap - *buf_len) {
    new_cap = *buf_len + copy_len + 1;
    if (!delim && new_cap < SIZE_MAX / 4)
      new_cap += new_cap / 2;
    tmp = scuffed_realloc(*buf_len, *buf, new_cap);
    if (!tmp)
      tmp = scuffed_realloc(*buf_len, *buf, cap + copy_len + 1);
    if (!tmp)
      return (1);
    *buf = tmp;
  }
  if (copy_len) {
    ft_memcpy(*buf + *buf_len, *read_pos, copy_len);
    read_pos += copy_len;
    *buf_len += copy_len;
  }
  return 0;
}
