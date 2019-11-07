#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "stack.h"
#include "general.h"

#include "enum.h"

struct reg_name
{
    char names[5]= "";
    int regist = 0;
};

void reg_init(reg_name* ptr)
    {
#define DEF_REG(name,num) strcpy(ptr[num].names,  #name);
#include "registers.h"
#undef DEF_REG
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
    char where_create_machine[50] =  "" ;
    strcat( where_create_machine,  machine_file_directory);
    strcat( where_create_machine,  machine_file_name);
    FILE* machine = fopen(  where_create_machine , "rb");
    if ( machine == nullptr)
        {
        printf("Can't open machine code file");
        exit(1);
        }
    FILE* out = fopen( out_name, "w");

    char readed[signature_version_long] = "";
    fseek( machine, signature_long-signature_version_long, SEEK_SET);
    fread(readed, sizeof(char),signature_version_long, machine);
    if (readed[0] != version)
        {
        printf("version conflict\n");
        exit(2);
        }

    fseek( machine, 0, SEEK_END);
    long count_sym = ftell(machine) + 1 - signature_long;
    char* code = ( char* ) calloc(count_sym, sizeof(char) );
    fseek( machine, signature_long, SEEK_SET);
    fread( code, sizeof(char), count_sym, machine);
    printf("%ld\n%s\n" , count_sym, code);

     long pc = 0;
     char current_command = -1;
     int current_arg = -1;
     stack_t cpu_stk = { "cpu_stk" };
     stack_initial( &cpu_stk );
     stack_create( &cpu_stk );

#define DEF_CMD(label,name,num,count_of_arguments,name_string,cmd_code)\
    if ( current_command == num )\
        {\
        if ( count_of_arguments != 0 )\
            {\
            current_arg = *( (int*) (code+pc) );\
            pc+=4;\
            }\
        cmd_code;\
        }

    while ( pc != -1 )
        {
        current_command = code[pc];
        pc++;
#define $ ,

#include "commands.h"
#undef $
        }

    stack_delete( &cpu_stk );
    free(code);
    fclose(machine);
    fclose(out);
    return 0;

    }