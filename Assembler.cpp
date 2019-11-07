#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <cmath>
#include "files.h"
#include "general.h"


#include "enum.h"

struct reg_name
    {
    char names[5]= "";
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

int label_joining( label_array_t* ptr, const char name[] )
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if( strcmp( name , ( *( ptr->data+i ) ).name ) == 0 )
            {
            return 0;
            }
        }
    strcpy( (char*) (ptr->data + ptr->current_size )  , name );
    ptr->current_size++;
    }

int label_find( label_array_t* ptr, const char name[] )
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if( strcmp( name , ( *( ptr->data+i ) ).name ) == 0 )
            {
            return i;
            }
        }
    }

void label_array_init( label_array_t* ptr, int  size = label_array_default_size )
    {
    assert(ptr);
    ptr->data = (label_t*) calloc(size, sizeof(label_t) );
    ptr->max_size = size;
    ptr->current_size = 0;

    assert(ptr);
    }

void reg_init(reg_name* ptr)
    {
#define DEF_REG(name,num) strcpy(ptr[num].names,  #name);
#include "registers.h"
#undef DEF_REG
    }


bool check_reg_argue(char reg_name[15])
    {
    #define DEF_REG(name, ind) && strcmp(#name,reg_name)
    bool check_mistake = 1
    #include "registers.h";
    #undef DEF_REG
    ;
    return check_mistake;
    }

int reg_num(char reg_name[15])
    {
    #define DEF_REG(name, ind) \
    if ( !strcmp(#name,reg_name) )\
        {\
         return REG__##name;\
        }

    #include "registers.h"

    #undef DEF_REG
    }

int main()
    {
    reg_name registries[register_count];
    reg_init(registries);
    long count_sym_in = -1;
    char* main_buffer = nullptr;
    buffer_read( (char**) &main_buffer, &count_sym_in, source_name );
    long line_count = lines( main_buffer, count_sym_in);

    char where_create_machine[50] =  "" ;
    strcat( where_create_machine,  machine_file_directory);
    strcat( where_create_machine,  machine_file_name);
    FILE* machine = fopen(  where_create_machine , "wb");

    char g[9] = "spirit!";
    g[7] = version;
    fwrite( g, sizeof( int ),2, machine );

    char current[30];
    char curr_cmd[15];
    int current_count = -1;
    int curr_num = -1;
    line* strings = nullptr;
    strings = (line*) calloc(line_count, sizeof(line));
    change(main_buffer,&count_sym_in, '\n','\0');
    fragmentation( strings, main_buffer, count_sym_in );

    int count = -1;
    int cmd_length = -1;
    int shift = 0;
    int current_arg = -1;
    char curr_arg[15] = "POISON";
    double double_arg = NAN;
    bool check_mistake;

#define DEF_CMD(label,name,num,count_of_arguments,name_string,cmd_code)\
    if ( strcmp(name_string,curr_cmd) == 0)\
        {\
        shift+=strlen(curr_cmd);\
        while( isspace(strings[i].begin_ptr[shift]) != 0)\
            {\
                shift++;\
            }\
        if ( strcmp( "PUSH" ,curr_cmd ) == 0 )\
            {\
            if (0 != sscanf(strings[i].begin_ptr + shift, "%lg", &double_arg ) )\
                {\
                curr_num = 1;\
                current_arg = round( (double_arg*accuracy_mul) );\
                }\
                    else\
                    {\
                    sscanf(strings[i].begin_ptr + shift, " %[a-z]s", curr_arg);\
                    check_mistake = check_reg_argue(curr_arg);\
                    if ( check_mistake )\
                        {\
                        printf(" invalid push registr argument");\
                        abort();\
                        }\
                    if ( !check_mistake )\
                        {\
                        curr_num = 10;\
                        current_arg = reg_num(curr_arg);\
                        }\
                    }\
            printf("%d %d %s\n",curr_num, current_arg, curr_arg);\
            fwrite( &curr_num, sizeof(char), 1, machine);\
            fwrite( &current_arg, sizeof(int), 1, machine);\
            continue;\
            }\
        if ( (count_of_arguments != 0)  )\
            {\
                curr_num = CMD__##name;\
                if (0 != sscanf(strings[i].begin_ptr + shift, "%lg", &double_arg ) )\
                    {\
                    current_arg = round( (double_arg*1000) );\
                    }\
                else\
                {\
                sscanf(strings[i].begin_ptr + shift, " %[a-z]s ", curr_arg);\
                check_mistake = check_reg_argue(curr_arg);\
                if ( check_mistake && !label )\
                    {\
                    printf(" invalid push registr argument");\
                    abort();\
                    }\
                 if ( !check_mistake && !label )\
                    {\
                    current_arg = reg_num(curr_arg);\
                    }\
                if ( label ) \
                    {  \
                    sscanf(strings[i].begin_ptr + shift, " %s ", curr_arg);\
                    current_arg = lable_array.data[label_find( &lable_array, curr_arg )].addres;\
                    }\
                }\
            printf("%d %d %s\n",curr_num, current_arg, curr_arg);\
            fwrite( &curr_num, sizeof(char), 1, machine);\
            fwrite( &current_arg, sizeof(int), 1, machine);\
            continue;\
            }\
        curr_num = CMD__##name;\
        fwrite( &curr_num, sizeof(char), 1, machine);\
        printf("%d %d %s\n",curr_num, current_arg, curr_arg);\
        }

    label_array_t lable_array;
    label_array_init( &lable_array );
    /*for (int i = 0; i < line_count; i++)
        {
        if ( strchr(strings[i].begin_ptr+shift,':') == nullptr )
            {
            continue;
            }
        sscanf(strings[i].begin_ptr,"%s%n", curr_cmd, &current_count);
        sscanf(strings[i].begin_ptr + current_count - strlen(curr_cmd), "%s", curr_arg );
        label_joining( &lable_array, curr_arg );
        }*/

    for (int  i = 0; i < line_count ; i++)
        {
        printf("%d\n", ftell(machine) - signature_long);
        /*if ( strchr(strings[i].begin_ptr+shift,':') != nullptr )
            {
            sscanf(strings[i].begin_ptr+shift,"%s%s%n",curr_cmd,curr_arg, &current_arg);
            if ( 1 )//strchr(strings[i].begin_ptr+shift,':') == (char*) strings[i].begin_ptr+current_arg-1 )
                {
                //printf("%s %s\n",curr_cmd,curr_arg);
                label_joining( &lable_array, curr_cmd);
                lable_array.data[label_find(&lable_array,curr_cmd)].addres = ftell(machine) - signature_long;
                printf("%d\n",lable_array.data[label_find(&lable_array,curr_cmd)].addres);
                continue;
                }
            }*/
        shift = 0;
        strcpy(curr_arg, "POISON");
        current_arg = 0;
        while( isspace(strings[i].begin_ptr[shift]) != 0)
            {
                shift++;
            }
        if ( strchr(strings[i].begin_ptr+shift,':') != nullptr )
            {
            sscanf(strings[i].begin_ptr+shift,"%s",curr_arg);
            label_joining( &lable_array, curr_arg);
            current_count = label_find(&lable_array,curr_cmd);
            lable_array.data[current_count].addres = ftell(machine) - signature_long;
            printf("Q%s %d\n", lable_array.data[current_count].name, lable_array.data[current_count].addres );
            continue;
            }
        if ( *(strings[i].begin_ptr+shift) == '\0')
            {
            continue;
            }
        current_count = sscanf(strings[i].begin_ptr+shift,"%s",curr_cmd);
        #include "commands.h"
        }

    printf("%d\n",lable_array.current_size );

    for ( int i = 0; i < lable_array.current_size; i++ )
        {
        printf("%s %d\n", lable_array.data[i].name, lable_array.data[i].addres );
        }

    free(lable_array.data);
    fclose( machine );
    free( main_buffer );
    free( strings);
    return 0;
    }
/*if ( strcmp("",curr_cmd) == 0)\
        {\
        continue;\
        }\*/