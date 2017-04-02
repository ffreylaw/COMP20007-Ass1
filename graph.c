/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Geoffrey Law
 * (Student ID: 759218)
 *
 * This module contains useful functions for graph manipulation and creation.
 *
 */
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

/* Returns a pointer to a new graph with order vertices */
Graph new_graph(int order) {
    Graph graph = NULL;

    /* Initialise all members in the graph
     */
    graph = (Graph)malloc(sizeof(struct graph_t));
    graph->order = order;
    graph->size = 0;
    graph->vertices = (Vertex)malloc(order * sizeof(struct vertex_t));

    return graph;
}

/* Returns whether aim and vertex are pointing to the same location */
bool ptr_eq(void *aim, void *vertex) {
    if (aim == vertex) {
        return true;
    }
    return false;
}

/* Returns whether aim and vertex have the same id */
bool id_eq(void *aim, void *vertex) {
    if (((Vertex)aim)->id == ((Vertex)vertex)->id) {
        return true;
    }
    return false;
}

/* Add the edge from v1 to v2 to graph */
void add_edge(Graph graph, int v1, int v2) {
    prepend(&((graph->vertices[v1]).out), &(graph->vertices[v2]));
    prepend(&((graph->vertices[v2]).in), &(graph->vertices[v1]));
}

/* Free the memory allocated to graph */
void free_graph(Graph graph) {
    int i;
    for (i = 0; i < graph->order; i++) {
        free_list((graph->vertices[i]).in);
        free_list((graph->vertices[i]).out);
        free((graph->vertices[i]).label);
    }
    free(graph->vertices);
    free(graph);
}
