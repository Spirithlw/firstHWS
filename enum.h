// enum.h is a enum of commands in my own processor

#ifndef _ENUM_H
#define _ENUM_H

#define DEF_CMD(label,name,num,count_of_arguments,name_string,cmd_code)\
CMD__ ##name = num,

enum commands

    {
#include "commands.h"
    };

#undef DEF_CMD

#define DEF_REG(name, ind)\
REG__##name = ind,

enum registers

    {
#include "registers.h"
    };

#undef DEF_REG

#endif _ENUM_H