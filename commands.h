//
// Created by maxim on 15.10.19.
//

#include "config.h"

#define CLEAR_CONSOLE printf("\e[1;1H\e[2J")

#define RESET_COLOR printf("\u001b[0m")

#define SET_COLOR(color) printf("\u001b[%dm", color);


//============================================
//COMMANDS
//============================================
//	name	n_args	overload	condition			arg_type	opcode  code

DEF_CMD (PUSH, 	1,  	CMD_ALT (!isalpha(arg[0]) && arg[0]!='[', 	IMMED, 		1, 	StackPush(stack, arg))
                  	CMD_ALT (arg[1] == 'x', 			REG, 		11, 	StackPush(stack, registers[arg]))
                  	CMD_ALT (arg[0]== '[' && arg[2]!='x', 		MEM_IMMED, 	21, 	StackPush(stack, mem[arg]))
                  	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	MEM_REG, 	31, 	StackPush(stack, mem[registers[arg]])))

DEF_CMD (POP, 	1, 	CMD_ALT (arg[1] == 'x', 			REG, 		2, 	registers[arg] = StackPop(stack, &status))
                	CMD_ALT (arg[0] == '[' && arg[2] != 'x', 	MEM_IMMED, 	22, 	mem[arg] = StackPop(stack, &satus))
                	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	MEM_REG, 	32, 	mem[registers[arg]] = StackPop(stack, &status)))

DEF_CMD (ADD, 	0, 	CMD_ALT (true, 					NO_ARG, 	3, 	StackPush(stack, StackPop(stack, &status) + StackPop(stack, &status))))

DEF_CMD (SUB, 	0, 	CMD_ALT (true, 					NO_ARG, 	4, 	StackPush(stack, StackPop(stack, &status) - StackPop(stack, &status))))

DEF_CMD (MUL, 	0, 	CMD_ALT (true, 					NO_ARG, 	5, 	StackPush(stack, StackPop(stack, &status) * StackPop(stack, &status))))

DEF_CMD (DIV,  	0, 	CMD_ALT (true, 					NO_ARG, 	6, 	StackPush(stack, StackPop(stack, &status) / StackPop(stack, &status))))

DEF_CMD (OUT, 	0, 	CMD_ALT (true, 					NO_ARG, 	15, 	printf("%d\n", StackPop(stack, &status))))

DEF_CMD (IN, 	0, 	CMD_ALT (true, 					NO_ARG, 	16, 	scanf("%d", arg); StackPush(stack, arg))

DEF_CMD	(GSET, 	1, 	CMD_ALT (!isalpha(arg[0]), 			IMMED, 		19, 	graphics[arg] = StackPop(stack, &status))
                 	CMD_ALT (arg[1] == 'x', 			REG, 		23, 	graphics[registers[arg]] = StackPop(stack, &status)))

DEF_CMD (GOUT, 	0, 	CMD_ALT (true, 					NO_ARG, 	20,	CLEAR_CONSOLE;
												    	for(int i = 0; i < GRAPHICS_BUFFER_LENGTH`; i++){
														if(i % GRAPHICS_LINE_LENGTH == 0) printf("\n");
														SET_COLOR(graphics[i]);
														printf(" ");
														RESET_COLOR;
												      	} 
													sleep(1);))

DEF_CMD (SQRT, 	0, 	CMD_ALT (true, 					NO_ARG, 	24, 	StackPush(stack, sqrt(StackPop(stack, &status)))))

DEF_CMD (RET,   0,	CMD_ALT (true,  				NO_ARG,		18,	array = array_start + StackPop(stack, &status)))


//============================================
//JUMPS
//============================================
//	 name		opcode	condition

DEF_JUMP (JUMP, 	7, 	true)

DEF_JUMP (JE, 		8, 	StackPop(stack, &status) == Stack(stack, &status))

DEF_JUMP (JNE, 		9, 	StackPop(stack, &status) != Stack(stack, &status))

DEF_JUMP (JA, 		10, StackPop(stack, &status) > StackPop(stack, &status))

DEF_JUMP (JAE, 		12, StackPop(stack, &status) >= StackPop(stack, &status))

DEF_JUMP (JB, 		13, StackPop(stack, &status) < StackPop(stack, &status))

DEF_JUMP (JBE, 		14, StackPop(stack, &status) <= StackPop(stack, &status))

DEF_JUMP (CALL, 	17, true)
