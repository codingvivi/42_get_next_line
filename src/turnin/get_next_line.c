/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/04 20:13:09 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *free_and_null(t_to_free *tgts);
void *scuffed_realloc(size_t old_size, void *ptr, size_t new_size);
void *gnl_memchr(int c, const void *src, size_t count);
void *ft_memcpy(void *dest, const void *src, size_t count);
int get_cpy_len(int fd, t_gnl_buf *sp, size_t *cpy_len, unsigned char **dlm);
int copy_line(unsigned char **read_pos, t_gnl_currop *currop,
              unsigned char **buf, size_t *buf_len);
int init_vars(unsigned char **sb_p, t_to_free *its_p, unsigned char **buf_p,
              size_t *cap_p);

char *get_next_line(int fd) {
  static unsigned int count;
  static t_gnl_buf strm;
  t_to_free items2f = (t_to_free){};
  t_gnl_currop curr = (t_gnl_currop){};

  printf("call number: %u", count++);

  if (init_vars(&strm.buf, &items2f, &curr.outbuf, &curr.cap))
    return (free_and_null(&items2f));
  while (1) {
    if (strm.rd_pos == strm.rd_end) {
      if (get_cpy_len(fd, &strm, &curr.copy_len, &curr.delim))
        return (free_and_null(&items2f));
    }
    assert(curr.copy_len);
    assert(curr.delim);
    if (copy_line(&strm.rd_pos, &curr, &curr.outbuf, &curr.cap))
      return (free_and_null(&items2f));
    if (curr.delim)
      break;
  }
  curr.outbuf[curr.len] = 0;
  return ((char *)curr.outbuf);
}

int init_vars(unsigned char **sb_p, t_to_free *its_p, unsigned char **buf_p,
              size_t *cap_p) {
  if (!*sb_p) {
    *sb_p = malloc((BUFSIZ + 1) * sizeof(char));
    its_p->ptrs[its_p->num++] = *sb_p;
    if (!sb_p)
      return (1);
  }
  *buf_p = malloc((BUFFER_SIZE + 1) * sizeof(char));
  if (!*buf_p)
    return (1);
  *cap_p = BUFFER_SIZE;
  its_p->ptrs[its_p->num++] = *buf_p;
  return (0);
}

int get_cpy_len(int fd, t_gnl_buf *sp, size_t *cpy_len, unsigned char **dlm) {
  const ssize_t read_result = read(fd, sp->buf, BUFFER_SIZE);

  assert(read_result);
  printf("%zu", read_result);
  if (read_result < e_r_normal_bounds)
    return 1;
  if (read_result != BUFFER_SIZE)
    sp->flags |= e_r_eof;
  sp->rd_pos = sp->buf;
  sp->rd_len = (size_t)read_result;
  sp->rd_end = sp->rd_pos + sp->rd_len;
  *dlm = gnl_memchr('\n', sp->buf, sp->rd_len);
  assert(dlm);
  if (*dlm)
    *cpy_len = (*dlm - sp->rd_pos) + 1;
  else
    *cpy_len = sp->rd_len;
  return 0;
}

/*
  I still know it's unhinged to pass struct AND individual members,
  but this is leagues more readable than 18 struct member calls.
  I wouldn't do this in projects outside of 42 ever, I promise.
*/
int copy_line(unsigned char **read_pos, t_gnl_currop *currop,
              unsigned char **buf, size_t *buf_len) {
  size_t cap = currop->cap;
  size_t copy_len = currop->copy_len;
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
    cap = new_cap;
  }
  if (copy_len) {
    ft_memcpy(*buf + *buf_len, *read_pos, copy_len);
    read_pos += copy_len;
    *buf_len += copy_len;
  }
  return 0;
}
