#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <map>
#include "string_funcs.h"
#include "config.h"

const char registers[N_REGISTERS][MAX_REGISTER_NAME_LENGTH] = {"ax", "bx", "cx", "dx"};

char* parseArg(char *buffer, char* container)
{
  size_t pos = 0;
  sscanf (buffer, "%s%n", container, &pos);
  buffer+=pos;
  return buffer;
}

void writeBinary (const char *binary, const char *filename, size_t total_bytes)
{
  assert(binary);
  assert (filename);

  FILE *file = fopen (filename, "wb");
  fwrite (binary, sizeof (char), total_bytes, file);
  fclose (file);
}

char *processJumps (char* buffer, size_t amount_of_lines, std::map<std::string, int> marks, char* binary){
  assert(buffer);
  assert(binary);

  char *binary_start = binary;
  char cmd_name[MAX_COMMAND_NAME_LENGTH] = "";
  char arg[MAX_ARG_LENGTH] = "";

  for (int i = 0; i < amount_of_lines; i++)
    {
      buffer = parseArg(buffer, cmd_name);

#define DEF_JUMP(cmd, opcode, code)\
if(strcmp(cmd_name, #cmd) == 0)\
  {\
    binary++;\
    buffer = parseArg(buffer, arg);\
    *((int *) binary) = marks[arg];\
    binary += sizeof (int);\
    break;\
  }

#define DEF_CMD(cmd, n_args, decision_tree)\
if (strcmp (cmd_name, #cmd) == 0)\
  {\
    binary++;\
    if(n_args)\
    {\
      buffer = parseArg(buffer, arg);\
      binary+=sizeof(int);\
    }\
  }

#include "commands.h"
#undef DEF_CMD
#undef DEF_JUMP
    }

  return binary_start;
}


char *createBinary (char *buffer, size_t amount_of_lines, size_t* total_bytes)
{
  assert(buffer);
  assert(total_bytes);

  std::map<std::string, int> marks;
  char *binary = new char[amount_of_lines * BYTES_ARRAY_COEFFICIENT]();

  char *binary_start = binary;
  char *buffer_start = buffer;

  char cmd_name[MAX_COMMAND_NAME_LENGTH] = "";
  char arg[MAX_ARG_LENGTH] = "";

  bool status = false;

  for (int i = 0; i < amount_of_lines; i++)
    {
      buffer = parseArg(buffer, cmd_name);
      if(cmd_name[0] == ':')
        {
          int position = binary - binary_start;
          std::string mark_name;
          mark_name.insert(0, cmd_name);
          marks.insert(std::make_pair(mark_name, position));
          continue;
        }
#define DEF_CMD(cmd, n_args, decision_tree)\
        if (strcmp (cmd_name, #cmd) == 0)\
          {\
            if(n_args) buffer = parseArg(buffer, arg);\
            decision_tree\
          }\
        else

#define DEF_JUMP(cmd, opcode, code)\
        if(strcmp(cmd_name, #cmd) == 0)\
        {\
          *binary = opcode;\
          binary++;\
          *((int *) binary) = 0;\
          binary += sizeof (int);\
          buffer = parseArg(buffer, arg);\
        }\
        else

#include "commands.h"
#undef DEF_CMD
#undef DEF_JUMP
      if(strcmp (cmd_name, "") == 0) continue;
      else
        {
          printf("Bad syntax! Command '%s' doesn't exist. \n", cmd_name); exit(-1);
        }
        memset (arg, 0, MAX_ARG_LENGTH - 1);
        memset (cmd_name, 0, MAX_COMMAND_NAME_LENGTH - 1);
    }
    *total_bytes = binary - binary_start;
    return processJumps (buffer_start, amount_of_lines, marks, binary_start);
}

int main (int argc, char *const argv[])
{
  char *input_file = 0;
  char *output_file = 0;

  if (argc == 5)
    {
      if (strcmp ("-i", argv[1]) == 0)
        {
          input_file = argv[2];
        }
      if (strcmp ("-o", argv[3]) == 0)
        {
          output_file = argv[4];
        }
    }
  else
    {
      printf ("ASM compiler v 0.0.1\nUse this utility to create machine code for CPU emulator.\nMade by Maxim Kokryashkin");
      return 0;
    }

  File input{};
  input = loadFile (input_file);

  size_t amount_of_lines = getNumberOfLines (input.raw_data);
  size_t total_bytes = 0;

  char *binary = createBinary (input.raw_data, amount_of_lines, &total_bytes);
  writeBinary (binary, output_file, total_bytes);

  free(binary);
  free(input.raw_data);
  free(input.data);
  return 0;
}