/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/02/03 13:14:32 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_ERR -1
#define READ_NONE_READ 0
#define READ_EOF READ_NONE_READ

void *gnl_memchr(int c, const void *src, size_t count) {
  const unsigned char *s = src;
  unsigned char uc = (unsigned char)c;

  while (count--) {
    if (*s == uc)
      return (void *)s;
    s++;
  }
  return NULL;
}
void free_all(t_to_free *tgts) {
  void **curr = &tgts->ptrs[0];

  if (!tgts)
    return;

  while (tgts->num) {
    if (*curr)
      free(*curr);
    *curr = NULL;
    curr++;
    tgts->num--;
  }
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
