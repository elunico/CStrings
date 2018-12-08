#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STRINGS_H
#define STRINGS_H

typedef struct
{
  char *data;
  // 256 bit hash plus null byte
  char *hash;
  size_t size;
  size_t refcnt;
} string_t;

typedef struct
{
  string_t *s;
  unsigned int start;
  unsigned int end;
} string_view_t;

typedef struct stt
{
  string_t *token;
  struct stt *next;
  int size;
} string_tokens_t;

// returned string is reference counted using strinc and strdec; strdec frees
// data on dec to 0
string_t *string(char *from);
void strinc(string_t *s);
void strdec(string_t *s);
int char_at(string_t *s, int idx);
// returned view requires cleanup with function `view_done(string_view_t*)`
string_view_t *view(string_t *s, unsigned int start, unsigned int end);
// destroys a string view
void view_done(string_view_t *view);
// does the same thing as `string(char*)`
string_t *stringify(string_view_t *view);
void print_view(string_view_t *view, FILE *f, int nl);
// does the same thing as `string(char*)`
string_t *substring(string_t *from, size_t start, size_t end);
int string_compare(string_t *a, string_t *b);
int starts_with(string_t *a, string_t *pat);
int ends_with(string_t *a, string_t *pat);
int rcontains(string_t *a, string_t *s);
int contains(string_t *a, string_t *sub);
// does the same thing as `string(char*)`
string_t *concat(string_t *a, string_t *b);
// does the same thing as `string(char*)`
string_t *trim(string_t *original);
// must be destroyed with `tokens_done(string_tokens_t*)`
string_tokens_t *tokenize(string_t *s, char delim);
// used to destroy a string_tokens_t *object returned by `tokenize(string_t*, char)`
void tokens_done(string_tokens_t *tokens);
string_t *get_token(string_tokens_t *toks, int idx);
// does the same thing as `string(char*)`
string_t *readline(FILE *source);
char *charp(string_t *s);

#endif