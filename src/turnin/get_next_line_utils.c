/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/01/20 21:26:49 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stddef.h>

unsigned char *gnl_strchrnul(const unsigned char *str, int c) {
  const unsigned char *end = str;
  size_t len;
  while (*end)
    end++;
  len = end - str;

  c = (unsigned char)c;
  if (!c)
    return ((unsigned char *)str + len);
  while (*str && *(unsigned char *)str != c)
    str++;
  return ((unsigned char *)str);
}

void *ft_memcpy(void *dest, const void *src, size_t count) {
  unsigned char *d;
  const unsigned char *s = src;

  if (!dest && !src && count)
    return (NULL);
  d = dest;
  while (count--)
    *d++ = *s++;
  return (dest);
}
