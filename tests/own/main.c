/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:44:30 by lrain             #+#    #+#             */
/*   Updated: 2026/01/13 06:25:45 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
  const char *default_path = "resources/Bee Movie Script.txt";
  const char *path;

  if (argc != 2) {
    path = default_path;
  } else {
    path = argv[1];
  }

  const int fd_test = open(path, O_RDONLY);

  for (int i = 0; i < 17; i++) {
    printf("%s", get_next_line(fd_test));
  }

  return (0);
}
