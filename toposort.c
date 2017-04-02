/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Geoffrey Law
 * (Student ID: 759218)
 *
 * This module provides all the topological sorting functionality to the graph.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "toposort.h"

/* Explores all nodes reachable from a particular node recursively */
void visit(Vertex vertex, bool *perma, bool *tempo, List *list) {
    if (tempo[vertex->id]) {
        fprintf(stderr, "Error: not a directed acyclic graph (DAG)\n");
        exit(EXIT_FAILURE);
    }
    if (!perma[vertex->id]) {
        tempo[vertex->id] = true;

        List node = vertex->out;
        while (node != NULL) {
            /* Visits all unvisited outgoing edges recursively
             */
            visit((Vertex)node->data, perma, tempo, list);
            node = node->next;
        }

        perma[vertex->id] = true;
        tempo[vertex->id] = false;
        prepend(list, vertex);
    }
}

/* Returns a list of topologically sorted vertices using the DFS method */
List dfs_sort(Graph graph) {
    List list = NULL;
    bool perma[graph->order];   /* permanent marker */
    bool tempo[graph->order];   /* temporary marker */
    int i;

    /* Sets all vertices unmarked
     */
    for (i = 0; i < graph->order; i++) {
        perma[i] = false;
        tempo[i] = false;
    }

    /* Iterates all vertices and visits unvisited vertex
     */
    for (i = 0; i < graph->order; i++) {
        if (!perma[i]) {
            visit(&(graph->vertices[i]), perma, tempo, &list);
        }
    }

    return list;
}

/* Returns a list of topologically sorted vertices using the Kahn method */
List kahn_sort(Graph graph) {
    List list = NULL;
    List set = NULL;
    int i;

    /* Adds vertex that has no incoming edges into set
     */
    for (i = 0; i < graph->order; i++) {
        if ((graph->vertices[i]).in == NULL) {
            prepend(&set, &(graph->vertices[i]));
        }
    }

    while (set != NULL) {
        Vertex vertex = pop(&set);  /* Removes a vertex from set */
        prepend(&list, vertex);     /* Adds current vertex into list */

        List node = vertex->out;
        while (node != NULL) {
            /* Removes the incoming edge of the node from the graph
             */
            del(id_eq, vertex, &(((Vertex)node->data)->in));

            /* Checks whether current node has other incoming edges
             */
            if (((Vertex)node->data)->in == NULL) {
                insert((Vertex)node->data, &set);
            }

            node = node->next;
        }
    }

    /* Checks whether the graph has incoming edges
     */
    for (i = 0; i < graph->order; i++) {
        if ((graph->vertices[i]).in != NULL) {
            fprintf(stderr, "Error: not a directed acyclic graph (DAG)\n");
            exit(EXIT_FAILURE);
        }
    }

    /* List needed to be reversed
     * since 'prepend' function previously used for list
     */
    list = reverse(list);

    return list;
}

/* Uses graph to verify vertices are topologically sorted */
bool verify(Graph graph, List vertices) {
    int i = 0, j;
    int vArray[graph->order];

    /* Converts the list of vertices into int array
     */
    while (vertices != NULL) {
        vArray[i++] = ((Vertex)vertices->data)->id;
        vertices = vertices->next;
    }

    /* Iterates all vertices in the graph
     */
    for (i = 0; i < graph->order; i++) {
        List node = (graph->vertices[i]).out;

        /* Iterates all outgoing edges from ith vertex in the graph
         */
        while (node != NULL) {
            int edgeIn = i;
            int edgeOut = ((Vertex)node->data)->id;

            /* Finding the priority of the node and its outgoing edge,
             * if node appears first then break otherwise return false
             */
            for (j = 0; j < graph->order; j++) {
                if (vArray[j] == edgeIn) {
                    break;
                }
                if (vArray[j] == edgeOut) {
                    return false;
                }
            }

            node = node->next;
        }
    }

    return true;
}
