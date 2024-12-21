#ifndef NAUTY_CALLER_H
#define NAUTY_CALLER_H

#include "nauty.h"

// Function to call nauty with directed subgraph src/dst arrays
void nautyCaller(int* src, int* dst, int n, int num_edges);

#endif // NAUTY_CALLER_H