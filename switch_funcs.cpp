//
// Created by maxim on 30.10.19.
//
#include <map>
#include "switch_funcs.h"
#include "config.h"

const std::map<char, int> registers = {{'a', 0}, {'b',1}, {'c', 2}, {'d', 3}};

char *writeIntToBinary (int value, char *binary)
{
  *((int *) binary) = value;
  binary += sizeof (int);

  return binary;
}

char *writeRegisterArgToBinary(char* arg, char* binary)
{
  auto search = registers.find(arg[0]);
  if (search != registers.end()) {
      binary = writeIntToBinary (registers.at(arg[0]), binary);
    }
  else
    {
      printf ("Bad parameter! Register '%s' doesn't exist! \n", arg);
      exit (-1); //TODO potential fuck up
    }
  return binary;
}

char* decisionTree (bool condition, int arg_type, int opcode, char *binary, char *arg)
{
  if (condition)
    {
      *binary = opcode;
      binary++;
      switch (arg_type)
        {
          case IMMED:
            binary = writeIntToBinary (atoi (arg), binary); break;
          case REG:
            binary = writeRegisterArgToBinary (arg, binary); break;
          case MEM_IMMED:
            binary = writeIntToBinary (atoi (arg + 1), binary); break;
          case MEM_REG:
            *(arg + MAX_REGISTER_NAME_LENGTH) = '\0';
            binary = writeRegisterArgToBinary (arg+1, binary); break;
          default:
            break;
        }
    }
    return binary;
}