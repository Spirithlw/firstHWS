#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct strs
    {
    char* ptr;
    int   len;
    };

long lines(char bufin1[], long cou)
    {
    assert( !(bufin1 == nullptr) );
    assert(cou >= 0);

    int ln = 0;
    for ( int i = 0; i < cou ; i++ )
        {
        if ( (*(bufin1+i)) == '\n')
            {
            ln++;
            }
        }

    return ln+1;
    }


void fillit ( strs* mas, char* bufin, long cou)
    {
    assert( !(bufin == nullptr) );
    assert( !(mas == nullptr) );

    long it = 0;
    int lenbf = 0;
    (mas+it)->ptr = bufin ;

    for ( int i = 0; i < (cou); i++ )
        {
        if ( (*(bufin+i)) == '\n' )
            {
            (mas+it)->len = 1+i-lenbf;
            lenbf = i;
            it++;
            (mas+it)->ptr = (bufin+i+1);
            }
        }

    }


    int strcmp1( strs* massiv1 , strs* massiv2)
        {
        assert( !( massiv1 == nullptr ) );
        assert( !( massiv2 == nullptr ) );

        return strcmp( massiv1->ptr, massiv2->ptr );
        }

int main()
    {
    printf("Hello,my friend.Today,we will sort the \"onegin\" poem.\nIn inputOn.txt we have original text, in outputOn.txt sorted and original texts. Love Pushkin, sort texts!");
    FILE* bfrin = fopen( "/home/egor/forclion2/DREC/ONEGIN/inputOn.txt", "rt");
    if ( bfrin == nullptr)
        {
        printf("Error: can't find input file");
        return 1;
        }
    fseek( bfrin, 0, SEEK_END);
    long count_sym_in = ftell(bfrin) + 1;
    rewind( bfrin );
    char* bfrin1 = (char*) calloc( count_sym_in, sizeof( char ));
    fread((char*) bfrin1,sizeof(char), count_sym_in, bfrin);
    //bfrin1[count_sym_in - 1] = '\n';
    int mass_index = lines( bfrin1, count_sym_in);
    strs ind_mass[ mass_index ] ;
    fillit( ind_mass, bfrin1 , count_sym_in );
    FILE* bfrout = fopen( "/home/egor/forclion2/DREC/ONEGIN/outputOn.txt", "w");
        if ( bfrout == nullptr)
        {
            printf("Error: can't find output file");
            return 1;
        }
     strs* strs1,strs2;
     for (int i = 0 ; i < count_sym_in; i++)
        {
         if ( bfrin1[i] == '\n' )
            {
                bfrin1[i] = '\0';
            }
        }
     qsort(ind_mass , mass_index , sizeof(strs), (int(*) (const void *, const void *)) strcmp1);
     for( int i = 0; i < mass_index; i++)
     {
         fprintf( bfrout, "%s\n" , ind_mass[i].ptr);

        }
     for (int i = 0 ; i < count_sym_in; i++)
        {
         if ( bfrin1[i] == '\0' )
            {
             bfrin1[i] = '\n';
            }
        }
     fprintf(bfrout,"\n\nORIGINAL\n\n%s", bfrin1);
     fclose(bfrin);
     fclose(bfrout);
     free(bfrin1);
     return 0;
    }