#include "cycleFounder.h"

typedef struct {
    int n; // count of unique variables
    vector *names;
    int **matrix; // adjacency matrix
    int *cnt, *p, *used;
    int cycleLength; // after dfs cycle will be stored in *p
    int cycleStart, cycleEnd;
} Graph;

void printGraph(Graph *g) {
    for (int i = 0; i < g->n; ++i) {
        for (int j = 0; j < g->n; ++j) {
            printf("%d ", g->matrix[i][j]);
        }
        printf("\n");
    }
}

void dfs(Graph *g, int v) {
    if (g->cycleStart != -1) return; // recursive break

    g->used[v] = 1;

    for (int u = 0; u < g->n; ++u) {
        if (g->matrix[v][u]) {
            if (g->used[u] == 0) {
                g->p[u] = v;
                dfs(g, u);
            } else if (g->used[u] == 1) {
                g->cycleEnd = v;
                g->cycleStart = u;
                return;
            }
            g->cnt[v] += g->cnt[u] + 1;
        }
    }

    g->used[v] = 2;
}

void gProcess(Graph *g) {
    g->used = (int *) malloc(g->n * sizeof(int));
    assert(g->used != NULL && "bad mem allocation");
    memset(g->used, 0, g->n * sizeof(int));

    g->cnt = (int *) malloc(g->n * sizeof(int));
    assert(g->cnt != NULL && "bad mem allocation");
    memset(g->cnt, 0, g->n * sizeof(int));

    g->p = (int *) malloc((g->n + 1) * sizeof(int));
    assert(g->p != NULL && "bad mem allocation");
    memset(g->p, -1, (g->n + 1) * sizeof(int));

    g->cycleLength = 0;
    g->cycleStart = -1;
    g->cycleEnd = -1;

    for (int v = 0; v < g->n; ++v) {
        if (!g->used[v]) {
            dfs(g, v);
        }
    }

    if (g->cycleStart != -1) {
        int cycle[g->n + 1];
        cycle[g->cycleLength++] = g->cycleStart;
        for (int v = g->cycleEnd; v != -1; v = g->p[v]) {
            cycle[g->cycleLength++] = v;
        }
        for (int i = g->cycleLength - 1; i >= 0; --i) {
            g->p[g->cycleLength - 1 - i] = cycle[i];
        }
    }
}

int findCycle(vector *names, int **table, int size, char *msg_good, char *msg_bad) {
    Graph *g = (Graph *) malloc(sizeof(Graph));
    assert(g != NULL && "bad mem allocate");

    g->n = size;
    g->names = names;
    g->matrix = table;

    gProcess(g);

    if (g->cycleLength) {
        if (msg_good != NULL) {
            printf("%s {", msg_good);
            for (int i = 0; i < g->cycleLength; ++i) {
                printf("'%s'", (char *) Vec.get(g->names, g->p[i]));
                if (i + 1 != g->cycleLength) {
                    printf(" -> ");
                } else {
                    printf("}");
                }
        }
        }
        return 1;
    } else {
        if (msg_bad != NULL) {
            printf("%s", msg_bad);
        }
        return 0;
    }
}