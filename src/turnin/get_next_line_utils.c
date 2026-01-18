/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 19:22:26 by lrain             #+#    #+#             */
/*   Updated: 2026/01/18 20:35:41 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stddef.h>

char *gnl_strchrnul(const char *str, int c) {
  const char *end = str;
  size_t len;
  while (*end)
    end++;
  len = end - str;

  c = (unsigned char)c;
  if (!c)
    return ((char *)str + len);
  while (*str && *(unsigned char *)str != c)
    str++;
  return ((char *)str);
}
