#include <stdio.h>
#include <stdlib.h>

#define MV 32
#define MINCLIQUE 4
#define MAXCLIQUE 6
#define USAGE "addv forbiddencliquesize"

#define DEBUG 0

/* input is a list of adjacency matrices generated by "showg -aq" */
int main(int argc, char *argv[])
{
    char *vstring;
    int i, j, max, b, sz, ct;
    char d;
    int v = 0;
    int am[MV][MV];
    int *startp3, *startkk, *p, *k;

    ct = 0;
    vstring = malloc(2*sizeof(char));
    startp3 = malloc(MV*sizeof(int));
    startkk = malloc(MV*sizeof(int));
    if (argc != 2) {
        fprintf(stderr, USAGE);
    }

    sz = atoi(argv[1]);

    while (fgets(vstring, 8, stdin) != 0) {
        v = atoi(vstring);
        i = 0;
        j = 0;
        while (i < v && (d = getchar()) != EOF) {
            if (d == '\n') {
                ++i;
                j = 0;
            } else {
                am[i][j] = atoi(&d);
                ++j;
            }
        }
        max = 2 << v;
        // Add a vertex and all possible edges and print the g6
        // representation if the graph is a (C4, Kk)-graph.
        for (i = 0; i < max; i++) {
            p = startp3;
            k = startkk;
            for (j = 0; j < v; j++) {
                b = (i >> j) & 1;
                am[v][j] = b;
                am[j][v] = b;

                if (b == 1) {
                    *(p++) = j;
                } else {
                    *(k++) = j;
                }
            }
            *p = v;
            *k = v;
            am[v][v] = 0;
            if (hasp3(v, am, startp3) == 0 &&
                         hasindset(sz-1, v, am, startkk) == 0) {
                printg6(v+1, am);
            }
        }
        if (++ct % 100 == 0) {
            fprintf(stderr, "addv: %d input graphs processed so far.\n", ct);
        }
    }
}

/* Determine if the vertices given by startp3 make a P3 in am. */
int hasp3(int v, int am[][MV], int *startp3)
{
    int i, j, k, ct;
    int *p, *r;

    p = startp3;
    if (DEBUG) fprintf(stderr, "\n");

    for (i = *p; i < v; i = *(++p)) {
        ct = 0;
        r = p+1;
        for (j = *r; j < v; j = *(++r)) {
            for (k = 0; k < v; k++) {

                if (am[i][k] == 1 && am[j][k] == 1) {
                    if (DEBUG) fprintf(stderr, "(%d, %d, %d) is a P3\n", i, j, k);
                    return 1;
                }
            }
            if (DEBUG) fprintf(stderr, "(%d, %d) do not form a P3\n", i, j);
        }
        //free(r);
    }
    //free(p);
    return 0;
}

/* Determine if the vertices given by startkk contain an independent
 * set of size sz in am. */
int hasindset(int sz, int v, int am[][MV], int *startkk)
{
    int i1, i2, i3, i4, i5, i6, k;
    int *p1, *p2, *p3, *p4, *p5, *p6, *d;

    p1 = malloc(sizeof(startkk));
    p2 = malloc(sizeof(startkk));
    p3 = malloc(sizeof(startkk));
    p4 = malloc(sizeof(startkk));
    p5 = malloc(sizeof(startkk));
    p6 = malloc(sizeof(startkk));
    if (DEBUG) {
        d = malloc(sizeof(startkk));
        d = startkk;
        k = *d;
        while (k < v) {
            fprintf(stderr, "%d ", k = *(d++));
        }
        fprintf(stderr, "\n");
    }
    p1 = startkk;
    switch (sz) {
        case 4:
         for (i1 = *p1; i1 < v; i1 = *(++p1)) {
          p2 = p1 + 1;
          if (DEBUG) fprintf(stderr, "l1: (%d, %d, %d, %d)", i1, *p2, *(p2+1), *(p2+2));
          for (i2 = ((*p2 > 0) ? *p2 : v); i2 < v; i2 = *(++p2)) {
           p3 = p2 + 1;
           if (am[i1][i2] == 0) {
            for (i3 = ((*p3 > 0) ? *p3 : v); i3 < v; i3 = *(++p3)) {
             p4 = p3 + 1;
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = ((*p4 > 0) ? *p4 : v); i4 < v; i4 = *(++p4)) {
               if (am[i1][i4] == 0 && am[i2][i4] == 0 &&
                   am[i3][i4] == 0) {
                if (DEBUG) fprintf(stderr, "(%d, %d, %d, %d)", i1, i2, i3, i4);
                return 1;
               }
              }
             }
            }
           }
          }
         }
         return 0;

        case 5:
         for (i1 = *p1; i1 < v; i1 = *(++p1)) {
          p2 = p1 + 1;
          if (DEBUG) fprintf(stderr, "l1: (%d, %d, %d, %d, %d)", i1, *p2, *(p2+1), *(p2+2), *(p2+3));
          for (i2 = ((*p2 > 0) ? *p2 : v); i2 < v; i2 = *(++p2)) {
           p3 = p2 + 1;
           if (am[i1][i2] == 0) {
            for (i3 = ((*p3 > 0) ? *p3 : v); i3 < v; i3 = *(++p3)) {
             p4 = p3 + 1;
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = ((*p4 > 0) ? *p4 : v); i4 < v; i4 = *(++p4)) {
               p5 = p4 + 1;
               if (am[i1][i4] == 0 && am[i2][i4] == 0 &&
                   am[i3][i4] == 0) {
                for (i5 = ((*p5 > 0) ? *p5 : v); i5 < v; i5 = *(++p5)) {
                 if (am[i1][i5] == 0 && am[i2][i5] == 0 &&
                     am[i3][i5] == 0 && am[i4][i5] == 0) {
                  return 1;
                 }
                }
               }
              }
             }
            }
           }
          }
         }
         if (DEBUG) fprintf(stderr, "Graph has no ind set of size %d", sz);
         return 0;

        case 6:
         for (i1 = *p1; i1 < v; i1 = *(++p1)) {
          p2 = p1 + 1;
          if (DEBUG) fprintf(stderr, "l1: (%d, %d, %d, %d, %d, %d)", i1, *p2, *(p2+1), *(p2+2), *(p2+3), *(p2+4));
          for (i2 = ((*p2 > 0) ? *p2 : v); i2 < v; i2 = *(++p2)) {
           p3 = p2 + 1;
           if (am[i1][i2] == 0) {
            for (i3 = ((*p3 > 0) ? *p3 : v); i3 < v; i3 = *(++p3)) {
             p4 = p3 + 1;
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = ((*p4 > 0) ? *p4 : v); i4 < v; i4 = *(++p4)) {
               p5 = p4 + 1;
               if (am[i1][i4] == 0 && am[i2][i4] == 0 &&
                   am[i3][i4] == 0) {
                for (i5 = ((*p5 > 0) ? *p5 : v); i5 < v; i5 = *(++p5)) {
                 p6 = p5 + 1;
                 if (am[i1][i5] == 0 && am[i2][i5] == 0 &&
                     am[i3][i5] == 0 && am[i4][i5] == 0) {
                  for (i6 = ((*p6 > 0) ? *p6 : v); i6 < v; i6 = *(++p6)) {
                   if (am[i1][i6] == 0 && am[i2][i6] == 0 &&
                       am[i3][i6] == 0 && am[i4][i6] == 0 &&
                       am[i5][i6] == 0) {
                    if (DEBUG) fprintf(stderr, "(%d, %d, %d, %d, %d, %d)", i1, i2, i3, i4, i5, i6);
                    return 1;
                   }
                  }
                 }
                }
               }
              }
             }
            }
           }
          }
         }
         return 0;
    }
}
/* Print a g6 representation of the graph given by am. */
void printg6(int v, int am[][MV])
{
    int c, r, i, sixbit;

    if (0 <= v && v <= 62)
        printf("%c", v + 63);
    else if (63 <= v && v <= 258047)
        printf("%c%c%c%c", 126, v >> 12, v >> 6, v);
    else if (258048 <= v && v <= 68719476735)
        printf("%c%c%c%c%c%c%c%c", 126, 126, v >> 30, v >> 24,
               v >> 18, v >> 12, v >> 6, v);
    else {
        fprintf(stderr, "I can't deal with that many vertices...\n");
        exit(EXIT_FAILURE);
    }

    i = 0;
    sixbit = 0;
    for (c = 1; c < v; c++) {
        for (r = 0; r < c; r++) {
            sixbit = (sixbit << 1) + am[r][c];
            ++i;
            if (i == 6) {
                printf("%c", 63 + sixbit);
                i = 0;
                sixbit = 0;
            }
        }
    }
    if (i != 0) {
        sixbit = sixbit << (6 - i);
        printf("%c", 63 + sixbit);
    }
    printf("\n");
}

/* Determine if am has a C4. */
int hasc4(int v, int am[][MV])
{
    int i, ct;

    int p = 0;

    for (i = 0; i < v; i++) {
        ct = 0;
        for (p = i + 1; p < v; p++) {
            if (am[i][p] == 1 && am[v-1][p] == 1) {
                ct++;
                if (ct == 2) return 1;
            }
        }
    }
    return 0;
}

/* Determine if the graph given by am has an independent set of size sz. */
int hasindsetnoptr(int sz, int v, int am[][MV])
{
    int i1, i2, i3, i4, i5, i6;

    //printf("%d", sz);
    switch (sz) {
        case 4:
         for (i1 = 0; i1 < v; i1++) {
          for (i2 = i1+1; i2 < v; i2++) {
           if (am[i1][i2] == 0) {
            for (i3 = i2+1; i3 < v; i3++) {
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = i3+1; i4 < v; i4++) {
               if (am[i1][i4] == 0 && am[i2][i4] == 0 && am[i3][i4] == 0)
                return 1;
               }
              }
             }
            }
           }
          }
         return 0;

        case 5:
         for (i1 = 0; i1 < v; i1++) {
          for (i2 = i1+1; i2 < v; i2++) {
           if (am[i1][i2] == 0) {
            for (i3 = i2+1; i3 < v; i3++) {
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = i3+1; i4 < v; i4++) {
               if (am[i1][i4] == 0 && am[i2][i4] == 0 && am[i3][i4] == 0) {
                for (i5 = i4+1; i5 < v; i5++) {
                 if (am[i1][i5] == 0 && am[i2][i5] == 0 && am[i3][i5] == 0 &&
                     am[i4][i5] == 0)
                  return 1;
                }
               }
              }
             }
            }
           }
          }
         }
         return 0;

        case 6:
         for (i1 = 0; i1 < v; i1++) {
          for (i2 = i1+1; i2 < v; i2++) {
           if (am[i1][i2] == 0) {
            for (i3 = i2+1; i3 < v; i3++) {
             if (am[i1][i3] == 0 && am[i2][i3] == 0){
              for (i4 = i3+1; i4 < v; i4++) {
               if (am[i1][i4] == 0 && am[i2][i4] == 0 &&
                   am[i3][i4] == 0) {
                for (i5 = i4+1; i5 < v; i5++) {
                 if (am[i1][i5] == 0 && am[i2][i5] == 0 &&
                     am[i3][i5] == 0 && am[i4][i5] == 0) {
                  for (i6 = i5+1; i6 < v; i6++) {
                   if (am[i1][i6] == 0 && am[i2][i6] == 0 &&
                       am[i3][i6] == 0 && am[i4][i6] == 0 &&
                       am[i5][i6] == 0)
                    return 1;
                  }
                 }
                }
               }
              }
             }
            }
           }
          }
         }
         return 0;

        default:
            fprintf(stderr, "Currently, this program only detects cliques of size between %d and %d, inclusive.\n", \
                    MINCLIQUE, MAXCLIQUE);
    }
}
