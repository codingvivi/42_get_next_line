/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:44:30 by lrain             #+#    #+#             */
/*   Updated: 2026/02/05 15:42:04 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
  const char *default_path = "tests/resources/external/Bee Movie Script.txt";
  const char *path;

  if (argc != 2) {
    path = default_path;
  } else {
    path = argv[1];
  }

  // printf("Ouggh ");
  const int fd_test = open(path, O_RDONLY);
  assert(fd_test != -1);
  for (int i = 1; i <= 100; i++) {
    printf("%d\t", i);
    char *line = get_next_line(fd_test);
    // assert(line);
    printf("%s", line);
  }
  printf("<<<END OF PRINT>>>\n\n");
  printf("Buffer size:%d\nMemory size: %d", BUFFER_SIZE, MEMSIZE_INIT);
  return (0);
}
