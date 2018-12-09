/**
 * Example using the strings.c library
 * This program uses function to read in a CSV (comma-separated values) file
 * and tokenize it according to the tabs along each line
 * It prints out the tokens of each line one line at a time
 * **/

#include "strings.h"
#include <errno.h>
#include <stdio.h>

void usage() {
  printf("usage: ./readcsv FILE_NAME\n");
}

int main(int argc, char const **argv) {
  if (!argv[1]) {
    usage();
    exit(1);
  }
  FILE *f = fopen(argv[1], "r");
  if (f == NULL) {
    strerror(errno);
    abort();
  }
  string_t *t;
  while ((t = readline(f)) != NULL) {
    string_tokens_t *tokens = tokenize(t, ',');
    if (tokens->size < 2) {
      tokens_done(tokens);
      strdec(t);
    } else {
      printf("===============================================\n");
      for (int i = 0; i < tokens->size; i++) {
        printf("| %s ", charp(get_token(tokens, i)));
      }
      printf("|\n");
      strdec(t);
      tokens_done(tokens);
    }
  }
  fclose(f);
  return 0;
}