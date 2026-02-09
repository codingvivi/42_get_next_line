/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:44:30 by lrain             #+#    #+#             */
/*   Updated: 2026/02/09 18:29:47 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LINES
#define LINES 18
#endif

#include "get_next_line.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  const char *default_path = "tests/resources/external/Bee Movie Script.txt";
  //  const char *default_path = "tests/resources/own/pagesize_test_8192.txt";
  //  const char *default_path = "tests/resources/own/1char.txt";
  //  const char *default_path = "tests/resources/own/1newline.txt";
  //  const char *default_path = "tests/resources/own/empty.txt";
  // const char *default_path = "tests/resources/external/variable_nl.txt";
  // const char *default_path = "tests/resources/external/giant_line.txt";
  const unsigned int default_lines = 18;
  char **endptr_num = NULL;

  const char *path;
  unsigned int lines;

  if (argc != 3) {
    path = default_path;
    lines = default_lines;
  } else {
    path = argv[1];
    lines = strtol(argv[2], endptr_num, 10);
  }

  // printf("Ouggh ");
  const int fd_test = open(path, O_RDONLY);
  assert(fd_test != -1);
  // char test[2];
  // printf("%zd", read(fd_test, test, 1));
  for (unsigned int i = 1; i <= lines; i++) {
    printf("%d\t", i);
    char *line = get_next_line(fd_test);
    // assert(line);
    printf("%s", line);
    free(line);
  }

  printf("<<<END OF PRINT>>>\n\n");
  printf("Buffer size:%d\nMemory size: %d", BUFFER_SIZE, MEMSIZE_INIT);
  return (0);
}
