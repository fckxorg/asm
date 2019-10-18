#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "string_funcs.h"
#include <assert.h>
#include "reference.h"

void writeBinary (const char *binary, const char *filename, size_t amount_of_commands)
{
  FILE *file = fopen (filename, "wb");
  fwrite (binary, sizeof (char), amount_of_commands * 4, file);
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

char *createBinary (FILE *file, size_t amount_of_commands)
{
  char *array = (char *) calloc (amount_of_commands * 32, sizeof (char));
  char *array_copy = array;
  char str[5] = "";

  for (int i = 0; i < amount_of_commands; i++)
    {
      fscanf (file, "%s", str);

#define DEF_CMD(name, num, bytes)\
        if (strcmp (str, #name) == 0)\
          {\
            sprintf (array, "%c", CMD_##name);\
            int value = 0;\
            array++;\
            for (int arg = 0; arg < (bytes - 1) / 4; arg++)\
            {\
              fscanf (file, "%d", &value);\
              *((int *) array) = value;\
              array += sizeof (int);\
            }\
            continue;\
        }

#include "commands.h"
#undef DEF_CMD
      printf ("Bad syntax! | \"%s\"", str);
    }
    return array_copy;
}

int main (int argc, char *const argv[])
{
  char *input_file = 0;
  char *output_file = 0;
  char *reference_file = 0;

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

  FILE *src_file = fopen (input_file, "r");

  char *binary = createBinary (src_file, amount_of_commands);
  writeBinary (binary, output_file, amount_of_commands);

  return 0;
}