#ifndef GRAPHLIB_H
#define GRAPHLIB_H

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
#include "../include/graphlib.h"
#include "../include/extralib.h"
#include "../include/cryptolib.h"
#include "../include/hashtab.h"

#define MAXVERTEX 1000
#define MAXINT 1000
#define MAXEDGE (((MAXVERTEX - 1) / 2) * MAXVERTEX)
#define _MAXEDGE(X) ((X * (X - 1)) / 2)

typedef struct RSA {
  unsigned long int P;
  unsigned long int Q;
  unsigned long int N;
  unsigned long int c;
  unsigned long int d;
  unsigned long int Phi;
} rsa_t;

struct EDGE {
  struct VERTEX* l_vertex;
  struct VERTEX* r_vertex;
};

struct VERTEX {
  int number;
  int color;
  unsigned long int R;
  rsa_t rsa;
  unsigned long int Z;
  char gcolor[256];
};

struct _GRAPH {
  struct VERTEX g_vertex[MAXVERTEX];
  struct EDGE g_edge[MAXEDGE];
  int edge_amount;
  int vertex_amount;
} GRAPH;

int vertex_check_connection(struct EDGE generated_edge);
void graph_save_graphviz(char *file);
void graph_save();
void graph_sort();
void edge_swap(int a, int b);
void vertex_swap(int position);
int edge_uniqueness(int generated_edge);
int edge_generation();
// int graph_generation();
void init_graph();
int init_color();
int init_check_connect();
int check_graph_coloring(int proof_amount);
void generate_R(int i);
int graph_coloring(int cur_vertex);
void graph_samples(int graph_choice);
void graph_generation(int graph_choice);
int search_vertex(int v);
void read_graph(char* filename);
#endif
