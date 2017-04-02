/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Clement Poh (cpoh@unimelb.edu.au)
 *
 * This module provides all the IO functionality related to graphs.
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "graphio.h"

#define MAX_LINE_LEN 256

/* Loads the graph from input */
Graph load_graph(char *input) {
    FILE *file;
    Graph graph = NULL;
    int i;
    int nVertex;
    int edgeIn, edgeOut;
    char str[MAX_LINE_LEN];

    file = fopen(input, "r");

    /* Reads the first line input n (number of vertex)
     * and creates / initialises a new graph
     */
    fscanf(file, "%d\n", &nVertex);
    graph = new_graph(nVertex);

    /* Reads all labels
     */
    for (i = 0; i < nVertex; i++) {
        fgets(str, MAX_LINE_LEN, file);
        str[strlen(str) - 1] = '\0';    /* Replaces '\n' to '\0' */

        (graph->vertices[i]).id = i;
        (graph->vertices[i]).label = (char*)malloc(strlen(str));
        strcpy((graph->vertices[i]).label, str);
        (graph->vertices[i]).in = NULL;
        (graph->vertices[i]).out = NULL;
    }

    /* Reads all edges
     */
    while (fscanf(file, "%d %d", &edgeIn, &edgeOut) == 2) {
        add_edge(graph, edgeIn, edgeOut);
        (graph->size)++;
    }

    fclose(file);

    return graph;
}

/* Prints the graph */
void print_graph(char *output, Graph graph) {
    FILE *file;
    int i;

    file = fopen(output, "w");

    /* Prints formatted output
     */
    fprintf(file, "digraph {\n");
    for (i = 0; i < graph->order; i++) {
        fprintf(file, "  %s", (graph->vertices[i]).label);

        List node = (graph->vertices[i]).out;
        if (node != NULL) {
            fprintf(file, " -> {");

            /* Prints all outgoing vertex labels of ith vertex
             */
            while (node != NULL) {
                print_vertex_label(file, (Vertex)node->data);
                node = node->next;
            }

            fprintf(file, "}");
        }
        fprintf(file, "\n");
    }
    fprintf(file, "}\n");

    fclose(file);
}

/* Prints the destination vertex label surrounded by spaces */
void print_vertex_label(FILE *file, void *vertex) {
    if (vertex)
        fprintf(file, " %s ", ((Vertex)vertex)->label);
}

/* Prints the id of a vertex then a newline */
void print_vertex_id(FILE *file, void *vertex) {
    if (vertex)
        fprintf(file, "%d\n", ((Vertex)vertex)->id);
}

/* Returns a sequence of vertices read from file */
List load_vertex_sequence(FILE *file, Graph graph) {
    const char *err_duplicate = "Error: duplicate vertex %d %s\n";
    const char *err_order = "Error: graph order %d, loaded %d vertices\n";
    List list = NULL;
    int id;

    while(fscanf(file, "%d\n", &id) == 1) {
        assert(id >= 0);
        assert(id < graph->order);

        if (!insert_if(id_eq, graph->vertices + id, &list)) {
            fprintf(stderr, err_duplicate, id, graph->vertices[id].label);
            exit(EXIT_FAILURE);
        }
    }

    if (len(list) != graph->order) {
        fprintf(stderr, err_order, graph->order, len(list));
        exit(EXIT_FAILURE);
    }

    return list;
}
