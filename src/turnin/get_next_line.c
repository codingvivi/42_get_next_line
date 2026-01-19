/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/19 18:14:42 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_ERR -1
#define READ_EOF 0

unsigned char *gnl_strchrnul(const unsigned char *str, int c);
void *ft_memcpy(void *dest, const void *src, size_t count);

char *get_next_line(int fd) {
  static t_gnl_buf stream;
  char *tmp;
  size_t newcap;
  size_t copylen;
  ssize_t read_result;

  // init vars
  if (BUFFER_SIZE <= 0)
    return NULL;
  if (!stream.bufcap) {
    stream = (t_gnl_buf){.bufcap = MEMSIZE_INIT,
                         .buf = malloc((stream.bufcap + 1) * sizeof(char))};
  }
  // while (1) {
  // }
  // if (!stream.buf)
  //   return NULL;

  read_result = read(fd, stream.buf, BUFFER_SIZE);
  // if (read_result == READ_EOF || read_result == READ_ERR)
  //   return NULL;
  // stream = (t_gnl_buf){.rpos = stream.buf};
  // stream.delim = gnl_strchrnul(stream.buf, '\n');
  // copylen = stream.delim - stream.rpos;
  // // increase if needed
  // if (stream.len + copylen >= stream.bufcap) {
  //   newcap = stream.len + copylen + 2;
  //   while ((stream.len + copylen) > newcap && newcap < SIZE_MAX / 4)
  //     newcap += newcap / 2;
  //   tmp = malloc(newcap * sizeof(char));
  //   if (!tmp) {
  //     newcap = stream.len + copylen + 2;
  //     tmp = malloc(newcap * sizeof(char));
  //     if (!tmp)
  //       return NULL;
  //   }
  // }
  // return (char *)stream.buf;
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
