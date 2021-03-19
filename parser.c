#include "parser.h"

#define MAX_ARRAY_SIZE 1000

int splitSyntax(char *srcFile, char dest[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], char *divs[56]) {

    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }

    char *src = malloc(MAX_ARRAY_SIZE * sizeof(char));
    while(fgets(src, MAX_ARRAY_SIZE, in)){
        fprintf(stdout,"\n%s\n",src);
        char tmpStr[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
        for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
            memset(tmpStr[i], 0, MAX_ARRAY_SIZE);
        }
        int i = 0;
        int k = 0;
        int z = 0;
        int Id = -1;
        int opF = 1; //is op flag for ch

        while (src[i] != '\n' && src[i] != '\0' && src[i] != '\r') {
            int dvF = 0;
            for (int j = 0; j < 56; j++) {
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

                ++k;
                i += (int) strlen(divs[Id]);
                opF = 1;
            }
        }

        int qeq = 0;
        for (i = 0; i < MAX_ARRAY_SIZE; ++i) {
            if ((int) strlen(tmpStr[i]) > 0) strcpy(dest[qeq++], tmpStr[i]);
        }
        for(i = 0; i<qeq; i++) {
            fprintf(stdout, "%s", dest[i]);
        }}

    return 0;
}