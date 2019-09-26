#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*struct str!
    {
    char* ptr;
    int len;
    }*/

int main()
    {
    char* bfr1 = nullptr;
    FILE* bfr = fopen("/home/egor/Документы/checking.txt", "rt");
    fseek(bfr, 0, SEEK_END);
    bfr1 = (char* ) calloc(ftell(bfr)+2, sizeof(char));

    long count_sym = ftell(bfr)+2;
        rewind(bfr);

        fread( (char*) bfr1,sizeof(char),count_sym,bfr);
        printf("%s \n ", bfr1);
        *(bfr1+ftell(bfr)+1) = '\n';
        printf("%d", count_sym);
    int count_lines = 0;
    /*for (int i = 0; i < count_sym ; i++)
        {
        printf("%c ", *(bfr1+i));
        if ( bfr1[i] = '\n' )
            count_lines++;
        }
    printf("%d",count_lines);*/

    free(bfr1);
    return 0;
    }
