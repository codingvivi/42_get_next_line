/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 23:12:23 by lrain             #+#    #+#             */
/*   Updated: 2026/01/18 20:32:01 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stddef.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE BUFSIZ
#endif

#ifndef READSIZE_INIT
#if BUFFER_SIZE > BUFSIZ
#define READSSIZE_INIT BUFFER_SIZE
#else
#define READSSIZE_INIT BUFSIZ
#endif
#endif

#define GNL_ERR 1
#define GNL_EOF 2
#define GNL_FOUND 4

typedef struct s_gnl_buf {
  int flags;
  size_t len;
  size_t capacity;
  char *rpos;
  char *buf;
} t_gnl_buf;

char *get_next_line(int fd);
