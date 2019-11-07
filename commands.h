//commands.h is a list of commands for my own processor


DEF_CMD(0,push,1,1,"PUSH",stack_push(&cpu_stk $ current_arg) )
DEF_CMD(0,pop,2,1,"POP",registries[current_arg].regist = stack_pop( &cpu_stk ) )
DEF_CMD(0,add,3,0,"ADD", stack_push( &cpu_stk $ stack_pop( &cpu_stk ) + stack_pop( &cpu_stk )  ))
DEF_CMD(0,sub,4,0,"SUB",stack_push( &cpu_stk $ stack_pop( &cpu_stk ) - stack_pop( &cpu_stk )  ))
DEF_CMD(0,mul,5,0,"MUL",stack_push( &cpu_stk $  ( stack_pop( &cpu_stk ) * stack_pop( &cpu_stk ) ) / accuracy_mul  ))
DEF_CMD(0,div,6,0,"DIV",stack_push( &cpu_stk $ round( ( (double) stack_pop( &cpu_stk ) / accuracy_mul ) / ( (double) stack_pop( &cpu_stk )  / accuracy_mul )  * accuracy_mul )  ))
DEF_CMD(0,sqrt,7,0,"SQRT",stack_push( &cpu_stk $ round( sqrt(( (double) stack_pop( &cpu_stk) / accuracy_mul ) )*accuracy_mul) ) )
DEF_CMD(0,sin,8,0,"SIN",stack_push( &cpu_stk $ round( sin(( (double) stack_pop( &cpu_stk) / accuracy_mul ) )*accuracy_mul) ) )
DEF_CMD(0,out,9,0,"OUT",fprintf( out, "%g\n", (double) stack_pop(&cpu_stk) / accuracy_mul ) )
DEF_CMD(0,push_registr,10,1,"PUSH",stack_push( &cpu_stk $ registries[current_arg].regist ) )
DEF_CMD(0,label,11,1,":",)
DEF_CMD(1,jmp,12,1,"JMP",pc = round( current_arg / accuracy_mul ) )
DEF_CMD(0,end,0,0,"END",pc = -1)