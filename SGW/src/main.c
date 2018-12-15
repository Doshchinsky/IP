#include "../include/main.h"

int _argc;
char** _argv;

int main(int argc, char** argv)
{
  _argc = argc;
  _argv = argv;
  srand(time(NULL));
  if (argc < 3) {
    fprintf(stderr, "%s[ERROR]%s\tNot enough arguments!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }

  int graph_choice = atoi(argv[1]);
  int proof_amount = atoi(argv[2]);

  if (proof_amount < 1) {
    fprintf(stderr, "%s[WARNING]%s\t %sCan't request specified amount of proofs. Set default value:%s\t%s3%s\n", RED, RESET, WHITE, RESET, GREEN, RESET);
    proof_amount = 3;
  }

  graph_generation(graph_choice);
  if (check_graph_coloring(proof_amount)) {
    printf("%s[ERROR]%s : Check graph is wrong!\n", RED, RESET);
  }
  return EXIT_SUCCESS;
}
