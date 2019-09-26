#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct strs
    {
    char* ptr;
    int len;
    };

long lines(char* bufin, long cou)
    {
    assert( !(bufin == nullptr) );
    assert(cou >= 0);
    int ln = 0;
    for ( int i = 0; i < cou ; i++ )
        {
        if ( (*(bufin+i)) == '\n')
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

    int compet( const strs* s1 , const strs* s2)
        {
        assert( !( s1 == nullptr ) );
        assert( !( s2 == nullptr ) );
        int i = 0, j = 0;
        while ( ( *((s1)->ptr+i) = *((s2)->ptr+j)) && (i<( ((s1)->len)-1 )) && (j<( ((s2)->len)-1 )) )
            {
            if (!(((int)('Z') > (int)(*((s1)->ptr+i)) > (int)('A')) || ((int)('z') > (int)(*((s1)->ptr+i)) > (int)('a'))))
                {
                i++;
                continue;
                }
                if (!(((int)('Z') > (int)(*((s2)->ptr+j)) > (int)('A')) || ((int)('z') > (int)(*((s2)->ptr+j)) > (int)('a'))))
                {
                    j++;
                    continue;
                }
                i++;
                j++;
            }
         if ( *((s1)->ptr+i) > *((s2)->ptr+j) )
             return 1;
         if ( *((s1)->ptr+i) < *((s2)->ptr+j) )
             return -1;
         if ( *((s1)->ptr+i) = *((s2)->ptr+j) )
             return 0;
        }

int main()
    {
    printf("Hello,my friend.Today,we will sort the \"onegin\" poem\nIn inputOn.txt we have original text, in outputOn.txt sorted and original texts.");
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
    bfrin1[count_sym_in - 1] = '\n';
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
     //qsort( ind_mass , mass_index , sizeof(strs), (int(*) (const void *, const void *)) compet);
     for( int i = 0; i < mass_index; i++)
        {
         fwrite(((ind_mass)->ptr+i), sizeof(strs), 1, bfrout);
        }
     bfrin1[count_sym_in - 1] = '\0';

     printf("%s", bfrin1);
     fprintf(bfrout,"%s", bfrin1);
     fclose(bfrin);
     fclose(bfrout);
     free(bfrin1);
     return 0;
    }