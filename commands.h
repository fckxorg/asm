//
// Created by maxim on 15.10.19.
//
//0 - no args
//1 - int args
//2 - str args
//DEF_CMD(name. n_args, CMD_ALT(condition, arg_type, opcode, code) CMD_ALT(condition, arg_type, opcode, code))

#define CMD_ALT(condition, arg_type, opcode, code)\
if(condition){\
  *array = opcode;\
      array++;\
  switch(arg_type){\
    case 1:\
      *((int *) array) = atoi(arg);\
      array += sizeof (int);\
      break;\
    case 2:\
      for(int reg =0 ; reg < 4; reg++)\
        {\
          if(strcmp(registers[reg], arg) == 0)\
            {\
              *((int *) array) = reg;\
              array+=sizeof(int);\
            }\
        }\
      break;\
    default:\
      continue;\
  }\
}

DEF_CMD(PUSH, 1, CMD_ALT(!isalpha(arg[0]), 1, 1, StackPush(stack, value)) CMD_ALT(isalpha(arg[0]), 2, 11, StackPush(stack, registers[arg])))
DEF_CMD(POP, 0, CMD_ALT(true, 0, 2, StackPop(stack, &status)))
DEF_CMD(ADD, 0, CMD_ALT(true, 0, 3, StackPush(stack, StackPop(stack, &status) + StackPop(stack, &status))))
DEF_CMD(SUB, 0, CMD_ALT(true, 0, 4, StackPush(stack, StackPop(stack, &status) - StackPop(stack, &status))))
DEF_CMD(MUL, 0, CMD_ALT(true, 0, 5, StackPush(stack, StackPop(stack, &status) * StackPop(stack, &status))))
DEF_CMD(DIV,  0, CMD_ALT(true, 0, 6, StackPush(stack, StackPop(stack, &status) / StackPop(stack, &status))))

