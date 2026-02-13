/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   musl-getdelim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:28:37 by lrain             #+#    #+#             */
/*   Updated: 2026/01/21 20:20:19 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "stdio_impl.h"
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

ssize_t getdelim(char **restrict strt_out, size_t *restrict capout, int delim,
                 FILE *restrict f) {
  char *tmp;
  unsigned char *delim_ptr;
  size_t c_to_copy;
  size_t lenout = 0;
  int c;

  FLOCK(f);

  if (!capout || !strt_out) {
    f->mode |= f->mode - 1;
    f->flags |= F_ERR;
    FUNLOCK(f);
    errno = EINVAL;
    return -1;
  }

  if (!*strt_out)
    *capout = 0;

  for (;;) {
    if (f->rpos != f->rend) {
      delim_ptr = memchr(f->rpos, delim, f->rend - f->rpos);
      c_to_copy = delim_ptr ? delim_ptr - f->rpos + 1 : f->rend - f->rpos;
    } else {
      delim_ptr = 0;
      c_to_copy = 0;
    }
    if (lenout + c_to_copy >= *capout) {
      size_t new_cap = lenout + c_to_copy + 2;
      if (!delim_ptr && new_cap < SIZE_MAX / 4)
        new_cap += new_cap / 2;
      tmp = realloc(*strt_out, new_cap);
      if (!tmp) {
        new_cap = lenout + c_to_copy + 2;
        tmp = realloc(*strt_out, new_cap);
        if (!tmp) {
          /* Copy as much as fits and ensure no
           * pushback remains in the FILE buf. */
          c_to_copy = *capout - lenout;
          memcpy(*strt_out + lenout, f->rpos, c_to_copy);
          f->rpos += c_to_copy;
          f->mode |= f->mode - 1;
          f->flags |= F_ERR;
          FUNLOCK(f);
          errno = ENOMEM;
          return -1;
        }
      }
      *strt_out = tmp;
      *capout = new_cap;
    }
    if (c_to_copy) {
      memcpy(*strt_out + lenout, f->rpos, c_to_copy);
      f->rpos += c_to_copy;
      lenout += c_to_copy;
    }
    if (delim_ptr)
      break;
    if ((c = getc_unlocked(f)) == EOF) {
      if (!lenout || !feof(f)) {
        FUNLOCK(f);
        return -1;
      }
      break;
    }
    /* If the byte read by getc won't fit without growing the
     * output buffer, push it back for next iteration. */
    if (lenout + 1 >= *capout)
      *--f->rpos = c;
    else if (((*strt_out)[lenout++] = c) == delim)
      break;
  }
  (*strt_out)[lenout] = 0;

  FUNLOCK(f);

  return lenout;
}

weak_alias(getdelim, __getdelim);
