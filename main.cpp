#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "string_funcs.h"
#include <assert.h>
#include "reference.h"

//#pragma pack 1 - упаковка с точностью до одного байта


struct WordBoundaries {
    char *start;
    char *end;
};

bool CheckArgs (char *input_file, char *output_file, char *reference_file)
{
  if (!input_file)
    {
      printf ("Input file was not provided.");
      return false;
    }
  if (!output_file)
    {
      printf ("Output file was not provided.");
      return false;
    }
  if (!reference_file)
    {
      printf ("Reference file was not provided.");
      return false;
    }
  return true;
}

void getWordsBoundaries (char *buffer, int buffer_size, WordBoundaries *index)
{
  /*!Writes list of pointers to string starts
   * @param file_data array with file_data
   * @param file_size length of file in symbols
   * @param string_starts array for pointers storing
   * */
  assert (buffer);
  assert (index);

  (*index).start = buffer;

  for (int i = 1; i < buffer_size; i++)
    {
      if (buffer[i - 1] == ' ' || buffer[i - 1] == '\n' || buffer[i - 1] == '\0')
        {
          buffer[i - 1] = '\0';
          (*index).end = &buffer[i - 2];
          index++;
          (*index).start = &buffer[i];
        }
    }

  buffer[buffer_size - 1] = '\0';
  (*index).end = &buffer[buffer_size - 2];
}

int getSizeOfBinary (char *raw_buffer)
{
  int count = 0;
  while (*raw_buffer)
    {
      if (*raw_buffer == ' ' || *raw_buffer == '\n' || *raw_buffer == '\0')
        {
          *raw_buffer = '\0';
          count++;
        }
      raw_buffer++;
    }
  return count;
}

int *createBinary (WordBoundaries *words, int binary_size)
{
  int *array = (int *) calloc (binary_size, sizeof (int));
  int *array_copy = array;

  for (int i = 0; i < binary_size; i++)
    {
      for (int j = 0; j < N_COMMANDS; j++)
        {
          if (strcmp (words[i].start, COMMANDS[j]) == 0)
            {
              *array = j;
              if (j < 2)
                {
                  array++;
                  *array = atoi (words[i+1].start);
                }
                break;
            }
        }
      i++;
      array++;
    }
  return array_copy;

}

int main (int argc, char *const argv[])
{
  char *input_file = 0;
  char *output_file = 0;
  char *reference_file = 0;

  if (argc == 7)
    {
      if (strcmp ("-i", argv[1]) == 0)
        {
          input_file = argv[2];
        }
      if (strcmp ("-o", argv[3]) == 0)
        {
          output_file = argv[4];
        }
      if (strcmp ("--ref", argv[5]) == 0)
        {
          reference_file = argv[6];
        }
    }
  else
    {
      printf ("ASM compiler v 0.0.1\nUse this utility to create machine code for CPU emulator.\nMade by Maxim Kokryashkin");
      return 0;
    }

  // if (!CheckArgs (input_file, output_file, reference_file))
  // {
  // return 1;
  // }
  File input{};
  input = loadFile (input_file);

  int binary_size = getSizeOfBinary (input.raw_data);

  auto words = (WordBoundaries *) calloc (binary_size, sizeof (WordBoundaries));
  getWordsBoundaries (input.raw_data, input.size, words);
  int *binary = createBinary (words, binary_size);

  return 0;
}