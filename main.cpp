#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "string_funcs.h"
#include <ctype.h>

struct Mark{
    size_t byte;
    char name[64];
};

const char registers[4][3] = {"ax", "bx", "cx", "dx"};

void writeBinary (const char *binary, const char *filename, size_t total_bytes)
{
  FILE *file = fopen (filename, "wb");
  fwrite (binary, sizeof (char), total_bytes, file);
  fclose (file);
}

size_t getAmountOfCommands (char *raw_buffer)
{
  size_t count = 0;
  while (*raw_buffer)
    {
      if (*raw_buffer == '\n')
        {
          count++;
        }
      raw_buffer++;
    }
  return count;
}


char *processJumps (FILE *file, size_t amount_of_commands, Mark* marks, char* array){
  Mark* cur_mark = marks;
  char *array_copy = array;
  char str[5] = "";
  char arg[64] = "";
  arg[63]= '\0';

  for (int i = 0; i < amount_of_commands; i++)
    {

      fscanf (file, "%s", str);
#define DEF_JUMP(cmd, opcode, code)\
if(strcmp(str, #cmd) == 0)\
{\
  array++;\
  fscanf(file, "%s", arg);\
      for(int j = 0; j < amount_of_commands; j++)\
        {\
          if(strcmp (marks[j].name, arg) == 0)\
            {\
              *((int*)array) = marks[j].byte;\
              array+=sizeof(int);\
              break;\
            }\
        }\
}

#define DEF_CMD(cmd, n_args, decision_tree)\
        if (strcmp (str, #cmd) == 0)\
          {\
            array++;\
            if(n_args)\
            {\
              fscanf(file, "%s", arg);\
              array+=sizeof(int);\
            }\
          }\

#include "commands.h"
#undef DEF_CMD
#undef DEF_JUMP
    }
  return array_copy;
}


char *createBinary (FILE *file, size_t amount_of_commands, size_t* total_bytes, Mark* marks)
{
  char *array = (char *) calloc (amount_of_commands * 32, sizeof (char));
  Mark* cur_mark = marks;
  char *array_copy = array;
  char str[5] = "";
  char arg[64] = "";
  arg[63]= '\0';
  bool status = false;

  for (int i = 0; i < amount_of_commands; i++)
    {
      fscanf (file, "%s", str);

      if(str[0] == ':')
        {
          cur_mark->byte = array - array_copy;
          memcpy (cur_mark->name, str+1, 64);
          cur_mark++;
        }
#define DEF_CMD(cmd, n_args, decision_tree)\
        if (strcmp (str, #cmd) == 0)\
          {\
            if(n_args)\
            {\
              fscanf (file, "%s", arg);\
            }\
            decision_tree\
          }\
        else

#define DEF_JUMP(cmd, opcode, code)\
        if(strcmp(str, #cmd) == 0)\
        {\
          *array = opcode;\
          array++;\
          if(strcmp(#cmd, "RET") != 0)\
          {\
            *((int *) array) = 0;\
            array += sizeof (int);\
          }\
          fscanf (file, "%s", arg);\
        }\
        else

#include "commands.h"
#undef DEF_CMD
#undef DEF_JUMP
      {
        printf("Bad syntax! Command '%s' doesn't exist. \n", str); exit(-1);
      }
          memset (arg, 0, 63);
          memset (str, 0, 4);
    }
    *total_bytes = array - array_copy;
    rewind(file);
    return processJumps (file, amount_of_commands, marks, array_copy);
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

  size_t amount_of_commands = getAmountOfCommands (input.raw_data);
  size_t total_bytes = 0;

  FILE *src_file = fopen (input_file, "r");
  Mark* marks = (Mark *) calloc (amount_of_commands, sizeof(Mark));

  char *binary = createBinary (src_file, amount_of_commands, &total_bytes, marks);
  writeBinary (binary, output_file, total_bytes);

  return 0;
}