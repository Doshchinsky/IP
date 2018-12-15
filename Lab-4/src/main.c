#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#include "../include/colorlib.h"
#include "../include/pokerlib.h"

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  if (argc < 2) {
    fprintf(stderr, "%s[ERROR]%s Not enough arguments!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }
  int n_players;
  n_players = atoi(argv[1]);
  mental_poker(n_players);
  return EXIT_SUCCESS;
}
