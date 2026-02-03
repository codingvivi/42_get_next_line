/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/02/03 15:01:48 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void free_all(t_to_free *tgts);
int gnl_init_vars(t_gnl_currop *cp, t_gnl_buf *sp);
int gnl_read(int fd, t_gnl_buf *sp);
void *gnl_memchr(int c, const void *src, size_t count);
int find_and_copy(t_gnl_buf *sp, t_gnl_currop *cp);
int ensure_space(t_gnl_currop **cpp, t_gnl_buf **spp);

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
    if (!strm.buf)
      free_all(&items2f);
    return NULL;
  }
  items2f.ptrs[items2f.num++] = strm.buf;
  while (1) {
    if (strm.rd_pos == strm.rd_end) {
      if (gnl_read(fd, &strm)) {
        free_all(&items2f);
        return NULL;
      }
    }
    if (find_and_copy(&strm, &curr)) {
      free_all(&items2f);
      return NULL;
    };
  }
}

int gnl_read(int fd, t_gnl_buf *sp) {
  ssize_t read_result;

  read_result = read(fd, sp->buf, BUFFER_SIZE);
  if (read_result < e_r_normal_bounds)
    return 1;
  if (read_result != BUFFER_SIZE)
    sp->flags |= e_r_eof;
  sp->rd_pos = sp->buf;
  sp->rd_len = (size_t)read_result;
  sp->rd_end = sp->rd_pos + sp->rd_len;
  return 0;
}

int find_and_copy(t_gnl_buf *sp, t_gnl_currop *cp) {

  cp->delim = gnl_memchr('\n', sp->buf, sp->rd_len);
  if (cp->delim)
    cp->copy_len = cp->delim - sp->rd_pos + 1;
  else
    cp->copy_len = sp->rd_len;
  if (ensure_space(&cp, &sp))
    return 1;
  return 0;
}

int ensure_space(t_gnl_currop **cpp, t_gnl_buf **spp) {

  t_gnl_currop *cp = **cpp;
  t_gnl_buf *sp = **spp;

  if (cp->cap - cp->len < cp->copy_len + 1) {
  }
  return 0;
}
