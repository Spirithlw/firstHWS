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

int label_joining( label_array_t* ptr, const char name[] ) //creation or joining by given label name
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
    return -1;
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
    return -1;
    }

int main()
    {
    reg_name registries[register_count];
    reg_init(registries);
    long count_sym_in = -1;
    char* main_buffer = nullptr;
    buffer_read( (char**) &main_buffer, &count_sym_in, source_name, 0, "rt" );
    long line_count = lines( main_buffer, count_sym_in);

    char where_create_machine[50] =  "" ;
    strcat( where_create_machine,  machine_file_directory);
    strcat( where_create_machine,  machine_file_name);
    FILE* machine = fopen(  where_create_machine , "wb");

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

#define DEF_CMD(name,num,count_of_arguments,label,regist_ex,ram_ex,name_string,cmd_code)\
    if ( strcmp(name_string,curr_cmd) == 0 )\
        {\
        shift+=strlen(curr_cmd);\
        while( isspace(strings[i].begin_ptr[shift]) != 0 )\
            {\
                shift++;\
            }\
         \
        if ( strcmp( "PUSH" ,curr_cmd ) == 0 )\
            {\
            if (0 != sscanf(strings[i].begin_ptr + shift, "%lg", &double_arg ) )\
                {\
                curr_num = 1;\
                current_arg = round( (double_arg*accuracy_mul) );\
                }\
                    else\
                    {\
                    if ( strstr( strings[i].begin_ptr + shift,"[") != nullptr )\
                        {\
                        curr_num = 11;\
                        if ( strstr( strings[i].begin_ptr + shift,"RAM") != nullptr )\
                            {\
                            curr_num = 21;\
                            }\
                         \
                        fwrite( &curr_num, sizeof(char), 1, machine);\
                        sscanf( strstr( strings[i].begin_ptr + shift,"[")+1,"%[^]]",current);\
                        current_count = 0;\
                        while( isspace( current[current_count] ) != 0)\
                            {\
                            current_count++;\
                            }\
                         \
                        sscanf(current+current_count, "%[a-z]" , curr_arg);\
                        current_arg = reg_num(curr_arg);\
                        fwrite( &current_arg, sizeof(int), 1, machine);\
                        printf("%d %d %s ",curr_num, current_arg, curr_arg);\
                        \
                        current_arg = 0;\
                        sscanf(current, "%[a-z+ \t]%n" , curr_arg,&current_count);\
                        sscanf(current+current_count, "%d" , &current_arg);\
                        current_arg*=accuracy_mul;\
                        fwrite( &current_arg, sizeof(int), 1,machine);\
                        printf("%d %s\n", current_arg,curr_arg);\
                        continue;\
                        }\
                     \
                    sscanf(strings[i].begin_ptr + shift, " %[a-z]s", curr_arg);\
                    check_mistake = check_reg_argue(curr_arg);\
                    if ( check_mistake )\
                        {\
                        printf(" invalid registr name");\
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
         \
        if ( (count_of_arguments != 0)  )\
            {\
            curr_num = CMD__##name;\
            fwrite( &curr_num, sizeof(char), 1, machine);\
            printf("%d", curr_num);\
            for ( int j = 0 ; j < count_of_arguments ; j++ )\
                {\
            if (0 != sscanf(strings[i].begin_ptr + shift, "%lg%n", &double_arg, &current_count ) )\
                {\
                current_arg = round( (double_arg*accuracy_mul) );\
                shift+=current_count;\
                while( isspace(strings[i].begin_ptr[shift]) != 0)\
                    {\
                    shift++;\
                    }\
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
                    current_arg = accuracy_mul*lable_array.data[label_find( &lable_array, curr_arg )].addres;\
                    }\
                shift+=strlen(curr_arg);\
                while( isspace(strings[i].begin_ptr[shift]) != 0)\
                    {\
                    shift++;\
                    }\
                }\
            printf(" %d %s", current_arg, curr_arg);\
            fwrite( &current_arg, sizeof(int), 1, machine);\
            }\
            printf("\n");\
            continue;\
            }\
        curr_num = CMD__##name;\
        fwrite( &curr_num, sizeof(char), 1, machine);\
        printf("%d %d %s\n",curr_num, current_arg, curr_arg);\
        continue;\
        }

    label_array_t lable_array;
    label_array_init( &lable_array );

    for (int  i = 0; i < line_count ; i++)
        {
        printf("N %ld\n", ftell(machine) );
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
            current_count = label_find(&lable_array,curr_arg);
            lable_array.data[current_count].addres = ftell(machine);
            continue;
            }
        if ( *(strings[i].begin_ptr+shift) == '\0')
            {
            continue;
            }
        current_count = sscanf(strings[i].begin_ptr+shift,"%s",curr_cmd);
        #include "commands.h"
        abort();
        }

    fclose( machine );
    machine = fopen(  where_create_machine , "wb");
    char g[9] = "spirit!";
    g[7] = version;
    fwrite( g, sizeof( int ),2, machine );
    printf("\n\n\n");

    for (int  i = 0; i < line_count ; i++)
        {
        printf("N %ld\n", ftell(machine) - signature_long);
        shift = 0;
        strcpy(curr_arg, "POISON");
        current_arg = 0;
        while( isspace(strings[i].begin_ptr[shift]) != 0)
            {
            shift++;
            }
        if ( strchr(strings[i].begin_ptr+shift,':') != nullptr )
            {
            continue;
            }
        if ( *(strings[i].begin_ptr+shift) == '\0')
            {
            continue;
            }
        current_count = sscanf(strings[i].begin_ptr+shift,"%s",curr_cmd);
        #include "commands.h"
        }

#undef DEF_CMD

    fclose(machine);
    free(lable_array.data);
    free( main_buffer );
    free( strings);
    return 0;
    }