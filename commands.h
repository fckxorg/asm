//
// Created by maxim on 15.10.19.
//

#include "config.h"

#define CLEAR_CONSOLE printf("\e[1;1H\e[2J")

#define RESET_COLOR printf("\u001b[0m")

#define SET_COLOR(color) printf("\u001b[%dm", color);


//==================
//Stack operations
//==================

#define POP StackPop(stack, &status)

#define PUSH(value), StackPush(stack, value)

#define READ scanf("%d", arg)

#define PRINT printf("%d\n", POP)

#define REGISTER registers[arg]

#define MEM_ACCESS_IMMED mem[arg]

#define MEM_ACCESS_REGISTER mem[REGISTER]

#define GRAPHICS_ARG graphics[arg]

#define GRAPHICS_REG graphics[reg]

//============================================
//COMMANDS
//============================================
//	name	n_args	overload	condition			arg_type	opcode  code

DEF_CMD (PUSH, 	1,  	CMD_ALT (!isalpha(arg[0]) && arg[0]!='[', 	IMMED, 		1, 	PUSH(arg))
                  	CMD_ALT (arg[1] == 'x', 			REG, 		11, 	PUSH(REGISTER))
                  	CMD_ALT (arg[0]== '[' && arg[2]!='x', 		MEM_IMMED, 	21, 	PUSH(MEM_ACCESS_IMMED))
                  	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	MEM_REG, 	31, 	PUSH(MEM_ACCESS_REGISTER)))

DEF_CMD (POP, 	1, 	CMD_ALT (arg[1] == 'x', 			REG, 		2, 	REGISTER = POP)
                	CMD_ALT (arg[0] == '[' && arg[2] != 'x', 	MEM_IMMED, 	22, 	MEM_ACCESS_IMMED = POP)
                	CMD_ALT (arg[0] == '[' && arg[2] == 'x', 	MEM_REG, 	32, 	MEM_ACCESS_REGISTER = POP))

DEF_CMD (ADD, 	0, 	CMD_ALT (true, 					NO_ARG, 	3, 	arg = POP + POP; PUSH(arg)))

DEF_CMD (SUB, 	0, 	CMD_ALT (true, 					NO_ARG, 	4, 	arg = POP - POP; PUSH(arg)))

DEF_CMD (MUL, 	0, 	CMD_ALT (true, 					NO_ARG, 	5, 	arg = POP * POP; PUSH(arg)))

DEF_CMD (DIV,  	0, 	CMD_ALT (true, 					NO_ARG, 	6, 	arg = POP/POP; PUSH(arg)))

DEF_CMD (OUT, 	0, 	CMD_ALT (true, 					NO_ARG, 	15, 	PRINT))

DEF_CMD (IN, 	0, 	CMD_ALT (true, 					NO_ARG, 	16, 	READ; PUSH(arg)))

DEF_CMD	(GSET, 	1, 	CMD_ALT (!isalpha(arg[0]), 			IMMED, 		19, 	GRAPHICS_ARG = POP)
                 	CMD_ALT (arg[1] == 'x', 			REG, 		23, 	GRAPHICS_REG = POP))

DEF_CMD (GOUT, 	0, 	CMD_ALT (true, 					NO_ARG, 	20,	CLEAR_CONSOLE;
												    	for(int i = 0; i < GRAPHICS_BUFFER_LENGTH`; i++){
														if(i % GRAPHICS_LINE_LENGTH == 0) printf("\n");
														SET_COLOR(GRAPHICS_ARG);
														printf(" ");
														RESET_COLOR;
												      	} 
													sleep(1);))

DEF_CMD (SQRT, 	0, 	CMD_ALT (true, 					NO_ARG, 	24, 	arg = sqrt(POP); PUSH(arg)))

DEF_CMD (RET,   0,	CMD_ALT (true,  				NO_ARG,		18,	array = array_start + POP))


//============================================
//JUMPS
//============================================
//	 name		opcode	condition

DEF_JUMP (JUMP, 	7, 	true)

DEF_JUMP (JE, 		8, 	POP == POP)

DEF_JUMP (JNE, 		9, 	POP != POP)

DEF_JUMP (JA, 		10, POP > POP)

DEF_JUMP (JAE, 		12, POP >= POP)

DEF_JUMP (JB, 		13, POP < POP)

DEF_JUMP (JBE, 		14, POP <= POP)

DEF_JUMP (CALL, 	17, true)
