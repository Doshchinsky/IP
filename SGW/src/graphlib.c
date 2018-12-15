#include "../include/graphlib.h"

char color_associate_array[4][10] = {"red", "green", "blue", "black"};
char new_associate_array[4][10] = {"red", "green", "blue", "black"};
char* color_array[4] = {RED, GREEN, BLUE, RESET};
char new_color_array[4][10] = {RED, GREEN, BLUE, RESET};

static int count_disconnect_vertex = 0;
static int* connect_vertex = NULL;
static int flag_two_vertex = 0;
extern char **_argv;
extern int _argc;

void graph_save_graphviz(char* file) {
  FILE* graph_file;
  char* filename = file;
  char out[256] = "";
  char cmd[256] = "dot -Tpng ";
  strcat(out, filename);
  strcat(out, ".png");
  graph_file = fopen(filename,"w+");
  if (graph_file == NULL) {
    fprintf(stderr,"%s[ERROR]%s\tCan't open file:\t<%s>\n", RED, RESET, filename);
    exit(EXIT_FAILURE);
  }
  fprintf(graph_file, "graph ipgraph {\n");
  // print vertex config
  for (int i = 0; i < GRAPH.vertex_amount; ++i)
    fprintf(graph_file, "%d [label=\"%d\", color=\"%s\"];\n", i+1, i+1, GRAPH.g_vertex[i].gcolor);

  // print edges
  for (int i = 0; i < GRAPH.edge_amount; ++i)
    fprintf(graph_file, "%d -- %d [label=\"%d_%d\", color=\"%s\"]\n",
                        GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number, GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number, "orange");

  fprintf(graph_file, "}\n");
  fclose (graph_file);
  strcat(cmd, filename);
  strcat(cmd, " -o ");
  strcat(cmd, out);
  system (cmd);
}

void generate_values() {
  unsigned long int euclid_res[3] = {0};
  int i = 0;
  int r = 0;
  int nod;
  int color_mask = 0x7;

  for (i = 0; i < 3; ++i) {
    while(1) {
      r = rand() % 3;
      if ((color_mask >> r) & 0x1) {
        color_mask &= ~(1 << r);
        break;
      }
    }
    strcpy(new_associate_array[i], color_associate_array[r]);
    strcpy(new_color_array[i], color_array[r]);
  }

  printf("\n%sDefault colors:%s\t\t%s%s%s\t%s%s%s\t%s%s%s\n", YELLOW, RESET,
         color_array[0], color_associate_array[0], RESET,
         color_array[1], color_associate_array[1], RESET,
         color_array[2], color_associate_array[2], RESET);
  printf("%sEncrypted colors:%s\t%s%s%s\t%s%s%s\t%s%s%s\n\n", YELLOW, RESET,
         new_color_array[0], new_associate_array[0], RESET,
         new_color_array[1], new_associate_array[1], RESET,
         new_color_array[2], new_associate_array[2], RESET);

  printf("\t%sENCRYPTED GRAPH:%s\n", CYAN, RESET);
  for (int i = 0; i < GRAPH.edge_amount; ++i) {
    printf("%sEDGE #%d:%s\t%s%d%s--%s%d%s\n", YELLOW, i, RESET, new_color_array[GRAPH.g_edge[i].l_vertex->color], GRAPH.g_edge[i].l_vertex->number, RESET,  new_color_array[GRAPH.g_edge[i].r_vertex->color], GRAPH.g_edge[i].r_vertex->number, RESET);
  }
  printf("\n");

  for (i = 0 ; i < GRAPH.vertex_amount; ++i) {
    do {
      do {
        generate_prime_number(2, MAXINT, &GRAPH.g_vertex[i].rsa.P);
        generate_prime_number(2, MAXINT, &GRAPH.g_vertex[i].rsa.Q);
     } while (GRAPH.g_vertex[i].rsa.P == GRAPH.g_vertex[i].rsa.Q);

      GRAPH.g_vertex[i].rsa.N = GRAPH.g_vertex[i].rsa.P * GRAPH.g_vertex[i].rsa.Q;
      GRAPH.g_vertex[i].rsa.Phi = (GRAPH.g_vertex[i].rsa.P - 1) * (GRAPH.g_vertex[i].rsa.Q - 1);

      GRAPH.g_vertex[i].rsa.d = 3;//generate_mutually_prime_number(Phi , 1, MAXINT);
      euclid(GRAPH.g_vertex[i].rsa.d, GRAPH.g_vertex[i].rsa.Phi, euclid_res);
      GRAPH.g_vertex[i].rsa.c = euclid_res[0];
      generate_R(i);
      expmod_func(GRAPH.g_vertex[i].R, GRAPH.g_vertex[i].rsa.d, GRAPH.g_vertex[i].rsa.N, &GRAPH.g_vertex[i].Z);

      nod = (GRAPH.g_vertex[i].rsa.c * GRAPH.g_vertex[i].rsa.d) % GRAPH.g_vertex[i].rsa.Phi;
    } while (GRAPH.g_vertex[i].rsa.c > 0xFFFFFF || GRAPH.g_vertex[i].rsa.d <= 1 || nod != 1 || (GRAPH.g_vertex[i].Z == 0 && GRAPH.g_vertex[i].R != 0));

    //printf("Vertex #%d, nod(%d)\tr=%s%lu %X %lu%s,\tP=%lu,\tQ=%lu,\tN=%lu,\tc=%lu,\td=%lu,\tZ=%lu\n", i + 1,nod, new_color_array[GRAPH.g_vertex[i].color], GRAPH.g_vertex[i].R, GRAPH.g_vertex[i].R, GRAPH.g_vertex[i].R & 0x3 , RESET, GRAPH.g_vertex[i].rsa.P, GRAPH.g_vertex[i].rsa.Q, GRAPH.g_vertex[i].rsa.N, GRAPH.g_vertex[i].rsa.c, GRAPH.g_vertex[i].rsa.d, GRAPH.g_vertex[i].Z);
  }
}

void generate_R(int i) {
  GRAPH.g_vertex[i].R = rand() % (GRAPH.g_vertex[i].rsa.Phi);
  if (!strcmp(new_associate_array[GRAPH.g_vertex[i].color], "red")) {
    GRAPH.g_vertex[i].R &= ~(0x3);
    GRAPH.g_vertex[i].R |= 0x0;
  } else if (!strcmp(new_associate_array[GRAPH.g_vertex[i].color], "green")) {
    GRAPH.g_vertex[i].R &= ~(0x3);
    GRAPH.g_vertex[i].R |= 0x1;
  } else if (!strcmp(new_associate_array[GRAPH.g_vertex[i].color], "blue")) {
    GRAPH.g_vertex[i].R &= ~(0x3);
    GRAPH.g_vertex[i].R |= 0x2;
  } else {
    GRAPH.g_vertex[i].R |= 0x3;
  }
}

void graph_save() {
  FILE* graph_file;
  char* filename = "graph.txt";
  graph_file = fopen(filename,"w+");
  if (graph_file == NULL) {
    fprintf(stderr,"%s[ERROR]%s\tCan't open file:\t<%s>\n", RED, RESET, filename);
    exit(EXIT_FAILURE);
  }
  fprintf(graph_file, "%d %d\n", GRAPH.vertex_amount, GRAPH.edge_amount);
  for (int i = 0; i < GRAPH.edge_amount; ++i)
    fprintf(graph_file, "%d %d\n", GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number);

  fclose (graph_file);
  graph_save_graphviz("graph_orig.dot");
}

void vertex_swap(int position) {
//  printf("A: %d -- %d\n", GRAPH.g_edge[position].l_vertex->number, GRAPH.g_edge[position].r_vertex->number);
  struct VERTEX* temporal_edge = GRAPH.g_edge[position].l_vertex;
  GRAPH.g_edge[position].l_vertex = GRAPH.g_edge[position].r_vertex;
  GRAPH.g_edge[position].r_vertex = temporal_edge;
//  printf("B:  %d -- %d\n", GRAPH.g_edge[position].l_vertex->number, GRAPH.g_edge[position].r_vertex->number);
}

void edge_swap(int a, int b)
{
  struct VERTEX* temporal_l_vertex;
  struct VERTEX* temporal_r_vertex;
  temporal_l_vertex = GRAPH.g_edge[a].l_vertex;
  temporal_r_vertex = GRAPH.g_edge[a].r_vertex;
  GRAPH.g_edge[a].l_vertex = GRAPH.g_edge[b].l_vertex;
  GRAPH.g_edge[a].r_vertex = GRAPH.g_edge[b].r_vertex;
  GRAPH.g_edge[b].l_vertex = temporal_l_vertex;
  GRAPH.g_edge[b].r_vertex = temporal_r_vertex;
}

void graph_sort() {
  for (int i = 0; i < GRAPH.edge_amount; ++i)
    if (GRAPH.g_edge[i].l_vertex->number > GRAPH.g_edge[i].r_vertex->number)
      vertex_swap(i);

  for (int i = 0; i < GRAPH.edge_amount; ++i)
    for (int j = 0; j < GRAPH.edge_amount; ++j)
      if (GRAPH.g_edge[i].l_vertex->number < GRAPH.g_edge[j].l_vertex->number)
        edge_swap(i, j);
      else if (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_edge[j].l_vertex->number)
        if (GRAPH.g_edge[i].r_vertex->number < GRAPH.g_edge[j].r_vertex->number)
          edge_swap(i, j);
}

int edge_uniqueness(int generated_edge) {
  for (int i = 0; i <= generated_edge; ++i)
    for (int j = i + 1; j <= generated_edge; ++j)
      if (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_edge[j].l_vertex->number && GRAPH.g_edge[i].r_vertex->number == GRAPH.g_edge[j].r_vertex->number)
        return 1;
      else if (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_edge[j].r_vertex->number && GRAPH.g_edge[i].r_vertex->number == GRAPH.g_edge[j].l_vertex->number)
        return 1;

  return 0;
}

int vertex_check_connection(struct EDGE generated_edge) {
  int c = 0;
  int i = 0;
  count_disconnect_vertex = 0;

  if (connect_vertex[generated_edge.l_vertex->number - 1] == 0){
    c |= 0x1;  }

  if (connect_vertex[generated_edge.r_vertex->number - 1] == 0){
    c |= 0x2;
  }
  for (i = 0; i < GRAPH.vertex_amount; ++i)
    if (connect_vertex[i] == 0)
      ++count_disconnect_vertex;
  if (c == 0x3) {
    if (flag_two_vertex == 0) {
      flag_two_vertex = 1;
    } else {
      return 2;
    }
  }
  if (c == 0x0 && count_disconnect_vertex > 0)
    return 1;
  if ((c & 0x1) > 0) connect_vertex[generated_edge.l_vertex->number - 1] = 1;
  if ((c & 0x2) > 0) connect_vertex[generated_edge.r_vertex->number - 1] = 1;
  return 0;
}

int vertex_check_connection_s(int curr_v)
{
  int i = 0;
  count_disconnect_vertex = 0;
  for (i = 0; i < GRAPH.edge_amount; ++i) {
    if (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_vertex[curr_v].number) {
      connect_vertex[GRAPH.g_edge[i].l_vertex->number - 1] = 1;
      connect_vertex[GRAPH.g_edge[i].r_vertex->number - 1] = 1;
      vertex_check_connection_s(GRAPH.g_edge[i].r_vertex->number - 1);
    }
  }
  for (i = 0; i < GRAPH.vertex_amount; ++i) {
    if (connect_vertex[i] == 0)
      ++count_disconnect_vertex;
  }
  return count_disconnect_vertex;
}

int edge_generation()
{
  int l;
  int r;
  for (int i = 0; i < GRAPH.edge_amount; ++i)
    do {
      l = (rand() % (GRAPH.vertex_amount));
      r = (rand() % (GRAPH.vertex_amount));

      GRAPH.g_edge[i].l_vertex = &GRAPH.g_vertex[l];
      GRAPH.g_edge[i].r_vertex = &GRAPH.g_vertex[r];

    } while(edge_uniqueness(i) || (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_edge[i].r_vertex->number) || (vertex_check_connection(GRAPH.g_edge[i]) > 0));


  return 0;
}

int init_check_connect() {
  count_disconnect_vertex = GRAPH.vertex_amount;
  connect_vertex = (int*)malloc(GRAPH.vertex_amount * sizeof(int));
  for (int i = 0; i < GRAPH.vertex_amount; ++i) {
    connect_vertex[i] = 0;
  }

  return 0;
}

void graph_samples(int graph_choice) {  switch (graph_choice) {
    case 1:
      GRAPH.vertex_amount = 5;
      GRAPH.edge_amount = 4;
      init_check_connect();
      init_graph();
      for (int j = 0; j < GRAPH.edge_amount; ++j) {
        GRAPH.g_edge[j].l_vertex = &GRAPH.g_vertex[j];
        GRAPH.g_edge[j].r_vertex = &GRAPH.g_vertex[j + 1];
      }
      for (int i = 0; i < GRAPH.edge_amount; ++i) {
        printf("%sEDGE #%d:%s\t%s%d--%d%s\n", YELLOW, i, RESET, WHITE,GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number, RESET);
      }
      return;
    case 2:
      init_check_connect();
      init_graph();
      edge_generation();
      graph_sort();
      for (int i = 0; i < GRAPH.edge_amount; ++i) {
        printf("%sEDGE #%d:%s\t%s%d--%d%s\n", YELLOW, i, RESET, WHITE,GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number, RESET);
      }
      return;
    case 3:
      init_check_connect();
      read_graph("graph.txt");
      graph_sort();
      for (int i = 0; i < GRAPH.edge_amount; ++i) {
        printf("%sEDGE #%d:%s\t%s%d--%d%s\n", YELLOW, i, RESET, WHITE,GRAPH.g_edge[i].l_vertex->number, GRAPH.g_edge[i].r_vertex->number, RESET);
      }
      return;
  }
}

void read_graph(char* filename) {
  FILE* graph_file;
  graph_file = fopen(filename, "r");
  int _vertex_amount = 0;
  int _edge_amount = 0;
  int l = 0;
  int r = 0;
  int i = 0;
  if (graph_file == NULL) {
    fprintf(stderr,"%s[ERROR]%s\tCan't open file:\t<%s>\n", RED, RESET, filename);
    exit(EXIT_FAILURE);
  }
  fscanf(graph_file, "%d", &_vertex_amount);
  fscanf(graph_file, "%d", &_edge_amount);
  if (_vertex_amount > MAXVERTEX) {
    fprintf(stderr, "%s[ERROR]%s\tVertexes overflow!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (_edge_amount > _MAXEDGE(_vertex_amount)) {
    fprintf(stderr, "%s[ERROR]%s\tEdges overflow!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (_edge_amount > _vertex_amount*_vertex_amount) {
    fprintf(stderr, "%s[ERROR]%s\tAmount of edges must be less than vertexes^2!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (_vertex_amount < _edge_amount - 1) {
    fprintf(stderr, "%s[ERROR]%s\tAmount of edges must be greater or equal vertexes - 1!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }
  GRAPH.vertex_amount = _vertex_amount;
  GRAPH.edge_amount = _edge_amount;

  init_graph();
  for (i = 0; i < _edge_amount; ++i) {

    fscanf(graph_file, "%d", &l);
    fscanf(graph_file, "%d", &r);
    GRAPH.g_edge[i].l_vertex = &GRAPH.g_vertex[search_vertex(l)];
    GRAPH.g_edge[i].r_vertex = &GRAPH.g_vertex[search_vertex(r)];

    if(edge_uniqueness(i) || (GRAPH.g_edge[i].l_vertex->number == GRAPH.g_edge[i].r_vertex->number)) {
      fprintf(stderr,"%s[ERROR]%s\tEdge %d -- %d\n", RED, RESET, l,r);
      exit(EXIT_FAILURE);
    }
  }

  if (vertex_check_connection_s(0) == 1) {
    printf("%s[ERROR]%s:\t GRAPH is invalid!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }
}

int search_vertex(int v) {
int i = 0;
  for (i = 0; i < GRAPH.vertex_amount; ++i) {
    if (GRAPH.g_vertex[i].number == v)
      return i;
  }
  fprintf(stderr,"%s[ERROR]%s\tVertex %d not found!\n", RED, RESET, v);
  exit(EXIT_FAILURE);
}

void init_graph() {
  int i = 0;
  for (i = 0; i < GRAPH.edge_amount; ++i) {
    GRAPH.g_edge[i].l_vertex = NULL;
    GRAPH.g_edge[i].r_vertex = NULL;
  }
  for (i = 0; i < GRAPH.vertex_amount; ++i) {
    GRAPH.g_vertex[i].number = i + 1;
    GRAPH.g_vertex[i].gcolor[0] = '\0';
    GRAPH.g_vertex[i].color = 3;//black
  }
}

int graph_coloring(int cur_vertex) {
  int i = 0;
  int j = 0;
  int color_mask = 0x7; // 111
  count_disconnect_vertex = 0;
  int error = 0;
  int result_coloring = 1;
  for (i = cur_vertex; i < GRAPH.vertex_amount; ++i) {
    color_mask = 0x7;
    for (j = 0; j < GRAPH.edge_amount; ++j) {
      if (GRAPH.g_edge[j].l_vertex->number == GRAPH.g_vertex[i].number) {
        if (GRAPH.g_edge[j].r_vertex->color == 0) { // red
          color_mask = color_mask & 0x6;
        } else if (GRAPH.g_edge[j].r_vertex->color == 1) { // green
          color_mask = color_mask & 0x5;
        } else if (GRAPH.g_edge[j].r_vertex->color == 2) { // blue
          color_mask = color_mask & 0x3;
        } else { // black
          color_mask = color_mask & 0x7;
        }
      }
      if (GRAPH.g_edge[j].r_vertex->number == GRAPH.g_vertex[i].number) {
        if (GRAPH.g_edge[j].l_vertex->color == 0) { // red
          color_mask = color_mask & 0x6;
        } else if (GRAPH.g_edge[j].l_vertex->color == 1) { // green
          color_mask = color_mask & 0x5;
        } else if (GRAPH.g_edge[j].l_vertex->color == 2) { // blue
          color_mask = color_mask & 0x3;
        } else { // black
          color_mask = color_mask & 0x7;
        }

      }
    }
    switch(color_mask) {
      case 0x1: // 001
        GRAPH.g_vertex[i].color = 0;
      break;

      case 0x2: // 010
        GRAPH.g_vertex[i].color = 1;
      break;

      case 0x4: // 100
        GRAPH.g_vertex[i].color = 2;
      break;

      case 0x3: // 011
        GRAPH.g_vertex[i].color = 0;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring)  break;
        GRAPH.g_vertex[i].color = 1;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring)  break;
        error = error | 1;
      break;

      case 0x5: // 101
        GRAPH.g_vertex[i].color = 0;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        GRAPH.g_vertex[i].color = 2;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        error = error | 1;
      break;

      case 0x6: // 110
        GRAPH.g_vertex[i].color = 1;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        GRAPH.g_vertex[i].color = 2;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        error = error | 1;
      break;

      case 0x7: // 111
        GRAPH.g_vertex[i].color = 0;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        GRAPH.g_vertex[i].color = 1;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        GRAPH.g_vertex[i].color = 2;
        result_coloring &= graph_coloring(i + 1);
        if (!result_coloring) break;
        error = error | 1;
      break;
      default: error = error | 1; break;
    }
    if (GRAPH.g_vertex[i].color >= 0 && GRAPH.g_vertex[i].color <= 3) {
      strcpy(GRAPH.g_vertex[i].gcolor, color_associate_array[GRAPH.g_vertex[i].color]);
    } else {
      strcpy(GRAPH.g_vertex[i].gcolor, "black");
    }
  }
  return error;
}

void graph_generation(int graph_choice) {
  GRAPH.vertex_amount = _argc < 4 ? 10 : atoi(_argv[3]);
  GRAPH.edge_amount = _argc < 5 ? GRAPH.vertex_amount : atoi(_argv[4]);

  if (GRAPH.vertex_amount > MAXVERTEX) {
    fprintf(stderr, "%s[ERROR]%s\tVertexes overflow!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (GRAPH.edge_amount > _MAXEDGE(GRAPH.vertex_amount)) {
    fprintf(stderr, "%s[ERROR]%s\tEdges overflow!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (GRAPH.edge_amount > GRAPH.vertex_amount*GRAPH.vertex_amount) {
    fprintf(stderr, "%s[ERROR]%s\tAmount of edges must be less than vertexes^2!\n", RED, RESET);
    exit(EXIT_FAILURE);
  } else if (GRAPH.edge_amount < GRAPH.vertex_amount - 1) {
    fprintf(stderr, "%s[ERROR]%s\tAmount of edges must be greater or equal vertexes - 1!\n", RED, RESET);
    exit(EXIT_FAILURE);
  }

  printf("\t%sCOLORLESS GRAPH:%s\n", CYAN, RESET);
  graph_samples(graph_choice);
  if (graph_coloring(0) == 1) {
    printf("%s[ERROR]%s:\t GRAPH is invalid!\n", RED, RESET);
    graph_save();
    exit(EXIT_FAILURE);
  }

  printf("\t%sCOLORLED GRAPH:%s\n", CYAN, RESET);
  for (int i = 0; i < GRAPH.edge_amount; ++i) {
    printf("%sEDGE #%d:%s\t%s%d%s--%s%d%s\n", YELLOW, i, RESET, color_array[GRAPH.g_edge[i].l_vertex->color], GRAPH.g_edge[i].l_vertex->number, RESET,  color_array[GRAPH.g_edge[i].r_vertex->color], GRAPH.g_edge[i].r_vertex->number, RESET);
  }
  graph_save();
  generate_values();
}

int check_graph_coloring(int proof_amount) {
  int error = 0;
  int index_Bob = 0;
  int success_proof = 0;
  int _proof_amount= proof_amount;
  int key_left_vertex_c_Alice = 0;
  int key_right_vertex_c_Alice = 0;
  unsigned long int left_color = 0;
  unsigned long int right_color = 0;

  if (_proof_amount == 0) error = error | 1;
  while (_proof_amount > 0) {
    index_Bob = rand() % GRAPH.edge_amount;
    //send index to Alice
    //Alice send keys N, c, Z
    key_left_vertex_c_Alice = GRAPH.g_edge[index_Bob].l_vertex->rsa.c;
    key_right_vertex_c_Alice = GRAPH.g_edge[index_Bob].r_vertex->rsa.c;
    //Bob calculate colors
    expmod_func(GRAPH.g_edge[index_Bob].l_vertex->Z, key_left_vertex_c_Alice, GRAPH.g_edge[index_Bob].l_vertex->rsa.N, &left_color);
    expmod_func(GRAPH.g_edge[index_Bob].r_vertex->Z, key_right_vertex_c_Alice, GRAPH.g_edge[index_Bob].r_vertex->rsa.N, &right_color);
    printf("%sPROOF%s %s#%d%s: Edge %s%d%s (%d -- %d) -> %s%lX%s - %s%lX%s (%lu - %lu)\n",
           YELLOW, RESET,
           GREEN, success_proof, RESET,
           YELLOW, index_Bob, RESET, GRAPH.g_edge[index_Bob].l_vertex->number, GRAPH.g_edge[index_Bob].r_vertex->number,
           new_color_array[left_color & 0x3], left_color, RESET,
           new_color_array[right_color & 0x3], right_color, RESET, left_color & 0x3, right_color & 0x3);
    left_color &= 0x3;
    right_color &= 0x3;
    printf("%lu == %lu\n", left_color , right_color);
    if (left_color == right_color) {
      error = error | 1;
      break;
    }
    --_proof_amount;
    ++success_proof;
  }
  printf("\n%sProofs amount%s  : %s%d%s\n", YELLOW, RESET, WHITE, proof_amount, RESET);
  printf("%sSuccess proofs%s : %s%d%s\n", YELLOW, RESET, GREEN, success_proof, RESET);
  return error;
}
