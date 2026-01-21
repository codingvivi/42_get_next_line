/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 23:12:23 by lrain             #+#    #+#             */
/*   Updated: 2026/01/21 21:07:21 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stddef.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE BUFSIZ
#endif

#ifndef MEMSIZE_INIT
#if BUFFER_SIZE > BUFSIZ
#define MEMSIZE_INIT BUFFER_SIZE
#else
#define MEMSIZE_INIT BUFSIZ
#endif
#endif

// wanted to do an enum, but no global vars allowed...
#ifndef READ_ERR
#define READ_ERR -1
#endif

#ifndef READ_NONEREAD
#define READ_NONE_READ 0
#endif

#ifndef STREAM_EOF
#define STREAM_EOF 1
#endif

/* enum e_buf_flags { gnl_eof = 1} */

typedef struct s_gnl_buf {
  int flags;
  size_t len;
  size_t cap;
  unsigned char *delim;
  unsigned char *buf;
  ssize_t read_len;
  unsigned char *read_pos;
  unsigned char *read_end;
} t_gnl_buf;

typedef struct s_gnl_currop {

  char *outbuf;
  size_t newcap;
  unsigned char *delim;
  size_t copy_len;
} t_gnl_currop;

char *get_next_line(int fd);
