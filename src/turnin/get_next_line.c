/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:26:53 by lrain             #+#    #+#             */
/*   Updated: 2026/01/18 20:29:19 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#import "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_ERR -1
#define READ_EOF 0

char *gnl_strchrnul(const char *str, int c);

char *get_next_line(int fd) {

  static t_gnl_buf stream;
  unsigned char *delim;
  ssize_t read_result;

  if (BUFFER_SIZE <= 0)
    return NULL;
  if (!stream.capacity) {
    stream = (t_gnl_buf){.capacity = rev_clamp(BUFSIZ, BUFFER_SIZE),
                         .buf = malloc((stream.capacity + 1) * sizeof(char))};
  }
  if (!stream.buf)
    return NULL;

  while (1) {
    read_result = read(fd, stream.buf, BUFFER_SIZE);
    if (read_result == READ_EOF || read_result == READ_ERR)
      return NULL;
  }
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
