/**
 * Example using the strings.c library
 * This program uses function to read in a TSV (tab-separated values) file
 * and tokenize it according to the tabs along each line
 * It prints out the tokens of each line one line at a time
 * **/

#include "strings.h"
#include <errno.h>
#include <stdio.h>

int main(int argc, char const **argv) {
  FILE *f = fopen("rgb.txt", "r");
  if (f == NULL) {
    strerror(errno);
    abort();
  }
  string_t *t;
  while ((t = readline(f)) != NULL) {
    string_tokens_t *tokens = tokenize(t, '\t');
    if (tokens->size < 2) {
      tokens_done(tokens);
      strdec(t);
    } else {
      for (int i = 0; i < tokens->size; i++) {
        printf("t: %s | ", charp(get_token(tokens, i)));
      }
      printf("\n");
      strdec(t);
      tokens_done(tokens);
    }
  }
  fclose(f);
}