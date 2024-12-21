#include "nautyCaller.h"
#include <stdio.h>

graph canon[MAXN * MAXM];
graph * nauty_g;
int* lab;
int* ptn;
int* orbits;

bool isConnected(int from, int to, int* src, int* dst, int num_edges) {
    for(int i = 0; i < num_edges; i++) {
        if(src[i] == from && dst[i] == to) return true;
    }
    return false;
}

void nautyCaller(int* src, int* dst, int subgraphSize, int num_edges) {
    int M = ((subgraphSize + WORDSIZE - 1) / WORDSIZE);
    
    //graph nauty_g[MAXN * MAXM];
    //int lab[MAXN], ptn[MAXN], orbits[MAXN];
    //int orbits[MAXN];

	// lab = new int[subgraphSize];
	// ptn = new int[subgraphSize];
	// nauty_g = new graph[subgraphSize * MAXM];
	// lab = new int[subgraphSize];
	// ptn = new int[subgraphSize];
	// orbits = new int[subgraphSize];

    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;
    setword workspace[160 * MAXM];
    graph canon[MAXN * MAXM];
    
    options.writeautoms = FALSE;
    options.getcanon = TRUE;
    options.defaultptn = TRUE;
    options.digraph = TRUE;
    
    //nauty_check(WORDSIZE, M, subgraphSize, NAUTYVERSIONID);
    
    // Initialize lab and ptn
    for (int i = 0; i < subgraphSize; i++) {
        lab[i] = i;
        ptn[i] = 1;
    }
    ptn[subgraphSize-1] = 0;
    
    // Initialize graph
    for (int i = 0; i < subgraphSize; i++) {
        set* gv = GRAPHROW(nauty_g, i, M);
        EMPTYSET(gv, M);
    }
    
    // Add edges
    for (int i = 0; i < num_edges; i++) {
        set* gv = GRAPHROW(nauty_g, src[i], M);
        ADDELEMENT(gv, dst[i]);
    }
    
    // Call nauty
    nauty(nauty_g, lab, ptn, NULL, orbits, &options, &stats, 
          workspace, 160*MAXM, M, subgraphSize, canon);
    
    printf("\nlab array (vertex -> position):\n");
    for(int i = 0; i < subgraphSize; i++) {
        printf("vertex %d -> position %d\n", i, lab[i]);
    }
    
    // Initialize and create inverse mapping
    int inv_lab[MAXN];
    for(int i = 0; i < subgraphSize; i++) {
        inv_lab[i] = -1;  // Initialize to invalid value
    }
    for(int v = 0; v < subgraphSize; v++) {
        int pos = lab[v];
        inv_lab[pos] = v;
    }
    
    printf("\ninv_lab array (position -> vertex):\n");
    for(int i = 0; i < subgraphSize; i++) {
        printf("position %d -> vertex %d\n", i, inv_lab[i]);
    }
          
    printf("\nChecking edges in canonical form:\n");
    for (int i = 0; i < subgraphSize-1; i++) {
        for(int j = 0; j < subgraphSize; j++) {
            if(i == j) continue;
            
            int from_vertex = inv_lab[i];
            int to_vertex = inv_lab[j];
            printf("Checking edge position %d->%d (vertex %d->%d): ", 
                   i, j, from_vertex, to_vertex);
            
            if(isConnected(from_vertex, to_vertex, src, dst, num_edges)) {
                printf("EDGE EXISTS\n");
            } else {
                printf("no edge\n");
            }
        }
    }
    printf("\n");
}