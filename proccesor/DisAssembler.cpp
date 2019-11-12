#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cmath>
#include "files.h"
#include "general.h"

struct reg_name
    {
    char names[reg_name_size]= "";
    };

struct label_t
    {
    char name[15]= "POTION";
    int addres = -1;
    };

struct label_array_t
    {
    label_t* data = nullptr;
    int current_size = 0;
    int max_size = label_array_default_size;
    };

void reg_init(reg_name* ptr)
    {
    #define DEF_REG(name,num) strcpy(ptr[num].names,  #name);
    #include "registers.h"
    #undef DEF_REG
    }

void label_array_init( label_array_t* ptr, int  size = label_array_default_size )
    {
    assert(ptr);
    ptr->data = (label_t*) calloc(size, sizeof(label_t) );
    ptr->max_size = size;
    ptr->current_size = 0;

    assert(ptr);
    }

int label_joining( label_array_t* ptr, int addr ) //creation or joining by given label name
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if(  ( *(ptr->data+i) ).addres == addr )
            {
            return 0;
            }
        }
    strcpy( (char*) (ptr->data + ptr->current_size )  , "label" );
    (*(ptr->data+ptr->current_size) ).addres = addr;
    ptr->current_size++;
    }

int label_find( label_array_t* ptr, int addr )
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if( (*(ptr->data+i) ).addres == addr )
            {
            return i;
            }
        }
    return -1;
    }

int main()
    {
    char where_create_machine[50] =  "" ;
    strcat( where_create_machine,  machine_file_directory);
    strcat( where_create_machine,  machine_file_name);
    long count_sym_in = -1;
    char* main_buffer = nullptr;
    buffer_read( (char**) &main_buffer, &count_sym_in, where_create_machine , signature_long, "rb");
    FILE* out = fopen( dis_name, "w");

    reg_name registries[register_count];
    reg_init(registries);
    label_array_t lable_array;
    label_array_init( &lable_array );
    int pc = 0;
    int curr_com = -1;
    int current_argument = 0;

#define DEF_CMD(name,num,count_of_arguments,label,regist_ex,name_string,cmd_code)\
    if ( curr_com == num )\
        {\
        if ( label )\
            {\
            current_argument = *( (int*) (main_buffer + pc) );\
            pc+=4;\
            label_joining( &lable_array, (current_argument / accuracy_mul ) );\
            continue;\
            }\
            pc+= 4*count_of_arguments;\
        }

    while( pc < count_sym_in-1 )
        {
        curr_com = main_buffer[ pc ];
        pc++;
        #include "commands.h"
        }

#undef DEF_CMD

    pc = 0;

#define DEF_CMD(cmd_name,num,count_of_arguments,label,regist_ex,name_string,cmd_code)\
    if ( curr_com == num )\
        {\
        fprintf( out, "%s ", name_string);\
        for (int i = 0; i < count_of_arguments; i++)\
            {\
            current_argument = *( (int*) (main_buffer + pc) );\
            if (regist_ex)\
                {\
                fprintf( out, "%s ", registries[current_argument].names);\
                pc+=4;\
                continue;\
                }\
            fprintf( out, "%g ", (double) current_argument / accuracy_mul );\
            pc+=4;\
            if ( label )\
                {\
                current_argument = label_find( &lable_array, (current_argument / accuracy_mul) );\
                fprintf(out, " - %s%d", lable_array.data[current_argument].name, current_argument );\
                }\
            }\
        fprintf(out, "\n");\
        }

    while ( pc < count_sym_in-1 )
        {
        if ( label_find( &lable_array, pc) != -1)
            {
            current_argument = label_find( &lable_array, pc);
            fprintf(out,"label%d\n", current_argument );
            }
        curr_com = main_buffer[pc];
        pc++;
        #include "commands.h"
        }

#undef DEF_CMD

    free(main_buffer);
    fclose(out);
    return 0;
    }