/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:44:30 by lrain             #+#    #+#             */
/*   Updated: 2026/01/20 21:31:19 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
  const char *default_path = "tests/own/resources/Bee Movie Script.txt";
  const char *path;

  if (argc != 2) {
    path = default_path;
  } else {
    path = argv[1];
  }

  // printf("Ouggh ");
  const int fd_test = open(path, O_RDONLY);
  assert(fd_test != -1);
  for (int i = 0; i < 2; i++) {
    char *line = get_next_line(fd_test);
    assert(line);
    printf("%s", line);
  }

  return (0);
}
