#include "parser.h"

#define MAX_ARRAY_SIZE 100
#define MAX_E_SIZE 1000

int rsyntax(char *srcFile, char dest[100][100], char *divs[56]) {

    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }

    char *src = malloc(100 * sizeof(char));
    fgets(src, 100, in);

    int lastValCh = (int) strlen(src) - 1;
    while (src[lastValCh] == ' ' || src[lastValCh] == '\n' || src[lastValCh] == '\t') src[lastValCh--] = '\0';

    char tmpStr[MAX_ARRAY_SIZE][100];
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        memset(tmpStr[i], 0, 100);
    }
    int i = 0;
    int k = 0;
    int z = 0;
    int Id = -1;
    int opF = 1; //is op flag for ch

    while (src[i] != '\n' && src[i] != '\0' && src[i] != '\r') {
        //while (i < strlen(src) - 1 && (src[i] == ' ' || src[i] == '\t')) i++;
        int dvF = 0;
        for (int j = 0; j < 35; j++) {
            char tmptmp[100];
            memset(tmptmp, 0, 100);
            for (int zzz = 0; zzz < strlen(divs[j]); zzz++) {
                tmptmp[zzz] = src[i + zzz];
            }
            tmptmp[strlen(divs[j])] = 0;
            if (!strcmp(tmptmp, divs[j])) {
                dvF = 1;
                Id = j;
                break;
            }
        }
        if (!dvF) {
            tmpStr[k][z++] = src[i++];
            opF = 0;
        } else {
            z = 0;
            if (!opF) k++;

            strcpy(tmpStr[k], divs[Id]);
            //tmpStr[k][0] = src[i];

            ++k;
            i += (int) strlen(divs[Id]);
            opF = 1;
        }
    }

    for (i = 0; i < MAX_ARRAY_SIZE; ++i) {
        strcpy(dest[i], tmpStr[i]);
        fprintf(stdout,"%s ",dest[i]);
    }

    //while ((dest[k][0] == ' ' || dest[k][0] == '\t')) k--;

    return k + (dest[k][0] != 0);
}