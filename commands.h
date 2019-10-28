//
// Created by maxim on 15.10.19.
//

#include "config.h"

#define CMD_ALT(condition, arg_type, opcode, code)\
if(condition){\
  *array = opcode;\
  array++;\
  \
  switch(arg_type){\
    case IMMED:\
      *((int *) array) = atoi(arg);\
      array += sizeof (int);\
      break;\
      \
    case REG:\
      status = false;\
      for(int reg = 0 ; reg < N_REGISTERS; reg++)\
        {\
          if(strcmp(registers[reg], arg) == 0)\
            {\
              *((int *) array) = reg;\
              array+=sizeof(int);\
              status = true;\
            }\
        }\
        if(!status)\
          {\
            printf("Bad parameter! Register '%s' doesn't exist! \n", arg); exit(-1);\
          }\
        break;\
        \
    case MEM_IMMED:\
      *((int *) array) = atoi(arg+1);\
      array += sizeof (int);\
      break;\
      \
    case MEM_REG:\
      status = false;\
      *(arg + MAX_REGISTER_NAME_LENGTH) = '\0';\
      for(int reg =0 ; reg < N_REGISTERS; reg++)\
        {\
          if(strcmp(registers[reg], arg+1) == 0)\
            {\
              *((int *) array) = reg;\
              array+=sizeof(int);\
              status = true;\
            }\
        }\
        if(!status)\
          {\
            printf("Bad parameter! Register '%s' doesn't exist! \n", arg+1); exit(-1);\
          }\
    default:\
      break;\
  }\
}

//============================================
//COMMANDS
//============================================
//	name	n_args	overload	condition			arg_type	opcode  code

DEF_CMD (PUSH, 	1,  	CMD_ALT (!isalpha(arg[0]) && arg[0]!='[', 	1, 		1, 	StackPush(stack, value))
                  	CMD_ALT (arg[1] == 'x', 			2, 		11, 	StackPush(stack, registers[arg]))
                  	CMD_ALT (arg[0]== '[' && arg[2]!='x', 		4, 		21, 	StackPush(stack, mem[atoi(arg+1)]))
                  	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	5, 		31, 	StackPush(stack, mem[registers[arg]])))

DEF_CMD (POP, 	1, 	CMD_ALT (arg[1] == 'x', 			2, 		2, 	registers[arg] = StackPop(stack, &status))
                	CMD_ALT (arg[0] == '[' && arg[2] != 'x', 	4, 		22, 	mem[atoi(arg+1)] = StackPop(stack, &status))
                	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	5, 		32, 	mem[registers[arg]] = StackPop(stack, &status)))

DEF_CMD (ADD, 	0, 	CMD_ALT (true, 					0, 		3, 	StackPush(stack, StackPop(stack, &status) + StackPop(stack, &status))))

DEF_CMD (SUB, 	0, 	CMD_ALT (true, 					0, 		4, 	StackPush(stack, StackPop(stack, &status) - StackPop(stack, &status))))

DEF_CMD (MUL, 	0, 	CMD_ALT (true, 					0, 		5, 	StackPush(stack, StackPop(stack, &status) * StackPop(stack, &status))))

DEF_CMD (DIV,  	0, 	CMD_ALT (true, 					0, 		6, 	StackPush(stack, StackPop(stack, &status) / StackPop(stack, &status))))

DEF_CMD (OUT, 	0, 	CMD_ALT (true, 					0, 		15, 	printf("%d\n", StackPop(stack, &status))))

DEF_CMD (IN, 	0, 	CMD_ALT (true, 					0, 		16, 	scanf("%d", value)))

DEF_CMD	(GSET, 	1, 	CMD_ALT (!isalpha(arg[0]), 			1, 		19, 	graphics[arg] = StackPop(stack, &status))
                 	CMD_ALT (arg[1] == 'x', 			2, 		23, 	graphics[registers[arg]] = StackPop(stack, &status)))

DEF_CMD (GOUT, 	0, 	CMD_ALT (true, 					0, 		20,	clrscr();
												for(int i = 0; i < GRAPHICS_BUFFER_SIZE; i++){
													if(i % GRAPHICS_LINE_SIZE == 0) printf("\n");
													printf("\u001b[%dm \u001b[0m", graphics[i]);
                                								}))

DEF_CMD (SQRT, 	0, 	CMD_ALT (true, 					0, 	24, 	StackPush(stack, sqrt(StackPop(stack, &status)))))


//============================================
//JUMPS
//============================================
//	 name		opcode	condition

DEF_JUMP (JUMP, 	7, 	true)

DEF_JUMP (JE, 		8, 	if(StackPop(stack, &status) == Stack(stack, &status)){buffer = 0})

DEF_JUMP (JNE, 		9, 	if(StackPop(stack, &status) != Stack(stack, &status)){buffer = 0})

DEF_JUMP (JA, 		10, 	if(StackPop(stack, &status) > StackPop(stack, &status)){buffer = 0})

DEF_JUMP (JAE, 		12, 	if(StackPop(stack, &status) >= StackPop(stack, &status)){buffer = 0})

DEF_JUMP (JB, 		13, 	if(StackPop(stack, &status) < StackPop(stack, &status)){buffer = 0})

DEF_JUMP (JBE, 		14, 	if(StackPop(stack, &status) <= StackPop(stack, &status)){buffer = 0})

DEF_JUMP (CALL, 	17, 	true)

DEF_JUMP (RET, 		18, 	true)
