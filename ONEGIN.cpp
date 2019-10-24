#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct strs
    {
    char* ptr;
    int   len;
    };

void buffer_read(FILE** fileptr, char** buffer , long* count_sym, const char file_name[])
    {
        *fileptr = fopen( file_name, "rt");
        if ( *fileptr == nullptr)
        {
            printf("Error: can't find input file");
            assert(0);
        }
        fseek( *fileptr, 0, SEEK_END);
        *count_sym = ftell(*fileptr) + 1;
        rewind( *fileptr );
        *buffer = (char*) calloc( *count_sym, sizeof( char ));
        fread(*buffer, sizeof(char), *count_sym, *fileptr);
        assert( *buffer );
    }

void change(char* main_buffer, long* length,  char s1, char s2)
    {
        for (int i = 0 ; i < *length; i++)
        {
            if (main_buffer[i] == s1 )
            {
                main_buffer[i] = s2;
            }
        }
    }

long lines(char bufin1[], long cou)
    {
    assert( bufin1 );
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
    assert( bufin );
    assert( mas );

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
        assert(  massiv1 );
        assert(  massiv2 );

        return strcmp( massiv1->ptr, massiv2->ptr );
        }

int main()
    {
    printf("Hello,my friend.Today,we will sort the \"onegin\" poem.\nIn inputOn.txt we have original text, in outputOn.txt sorted and original texts. Love Pushkin, sort texts!");
    FILE* file_buffer = nullptr;
    long count_sym_in = -1;
    char* main_buffer = nullptr;
    buffer_read( (FILE**) &file_buffer, (char**) &main_buffer, &count_sym_in, "/home/egor/forclion2/DREC/ONEGIN/inputOn.txt");
    int mass_index = lines( main_buffer, count_sym_in );
    strs ind_mass[ mass_index ] ;
    fillit(ind_mass, main_buffer , count_sym_in );
    FILE* bfrout = fopen( "/home/egor/forclion2/DREC/ONEGIN/outputOn.txt", "w");
        if ( bfrout == nullptr)
        {
            printf("Error: can't find output file");
            return 1;
        }

        change(  main_buffer, &count_sym_in, '\n', '\0');

     qsort(ind_mass , mass_index , sizeof(strs), (int(*) (const void *, const void *)) strcmp1);
     for( int i = 0; i < mass_index; i++)
     {
         fprintf( bfrout, "%s\n" , ind_mass[i].ptr);

        }

        change(  main_buffer, &count_sym_in, '\0', '\n');

     fprintf(bfrout, "\n\nORIGINAL\n\n%s", main_buffer);

     fclose(file_buffer);
     fclose(bfrout);
     free(main_buffer);

     return 0;

    }