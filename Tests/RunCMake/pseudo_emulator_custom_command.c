#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Usage:
//
//  /path/to/program arg1 [arg2 [...]]
//
// Return EXIT_SUCCESS if 'generated_exe' string was found in <arg1>

int main(int argc, char * argv[] )
{
  if (argc < 2)
    {
    return EXIT_FAILURE;
    }
  const char* substring = "generated_exe";
  const char* string = argv[1];
  if (strstr(string, substring) != 0)
    {
    return EXIT_SUCCESS;
    }
  fprintf(stderr, "Failed to find string '%s' in '%s'\n", substring, string);
  return EXIT_FAILURE;
}
