/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   musl-getdelim-commented.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:28:37 by lrain             #+#    #+#             */
/*   Updated: 2026/01/18 20:35:45 by lrain            ###   ########.fr       */
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

  // if no room in output or output nullptr
  if (!capout || !strt_out) {
    f->mode |= f->mode - 1;
    f->flags |= F_ERR;
    FUNLOCK(f);
    errno = EINVAL;
    return -1;
  }

  // if contents of output null null term
  // set output capacity to 0
  if (!*strt_out)
    *capout = 0;

  // endlessly
  // doesnt check conditionn every single bite
  for (;;) {
    // if were not at end of file yet
    if (f->rpos != f->rend) {
      // get ptr to delim
      // or null if not found i think
      delim_ptr = memchr(f->rpos, delim, f->rend - f->rpos);
      // if you didnt get a null ptr
      // chars to copy are delim_ptr - current pos + 1
      // else its until the end of the file
      c_to_copy = delim_ptr ? delim_ptr - f->rpos + 1 : f->rend - f->rpos;
    } else {
      // else set to null
      delim_ptr = 0;
      c_to_copy = 0;
    }
    //  if length out + c to copy larger or equals max cap
    if (lenout + c_to_copy >= *capout) {
      // +1 for null term, +1 for safety
      size_t new_cap = lenout + c_to_copy + 2;
      // if we havent found the delim
      // and there is enough space,
      // optimize realloc
      // size max / 2 would match hitting limit exactly
      // leaving no room for rest of operations extending it
      // power of 2 is faster to calc
      if (!delim_ptr && new_cap < SIZE_MAX / 4)
        // geometric growth
        new_cap += new_cap / 2;
      tmp = realloc(*strt_out, new_cap);
      // if realloc failed
      // try unoptimized
      if (!tmp) {
        new_cap = lenout + c_to_copy + 2;
        tmp = realloc(*strt_out, new_cap);
        if (!tmp) {
          /* Copy as much as fits and ensure no
           * pushback remains in the FILE buf. */
          c_to_copy = *capout - lenout;
          memcpy(*strt_out + lenout, f->rpos, c_to_copy);
          // move rpos accordingly
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
    // if there are characters co copy
    if (c_to_copy) {
      // copy to end, which is start + length already
      // copy from write position
      memcpy(*strt_out + lenout, f->rpos, c_to_copy);
      // move write position and increase length of output
      f->rpos += c_to_copy;
      lenout += c_to_copy;
    }
    // if its not null term we found the delim
    if (delim_ptr)
      break;
    // get one more char.
    // if its eof..
    if ((c = getc_unlocked(f)) == EOF) {
      // and we read nothing
      // or feof returned an error, so its a read error
      if (!lenout || !feof(f)) {
        FUNLOCK(f);
        // return error
        return -1;
      }
      break;
    }
    /* If the byte read by getc won't fit without growing the
     * output buffer, push it back for next iteration. */
    if (lenout + 1 >= *capout)
      // undo read. else from earlier wont 0 out.
      *--f->rpos = c;
    else if (((*strt_out)[lenout++] = c) == delim)
      // if we found the delim
      break;
  }
  // write null term
  (*strt_out)[lenout] = 0;

  FUNLOCK(f);

  return lenout;
}

weak_alias(getdelim, __getdelim);
