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

string_t *string(char *from);
void strinc(string_t *s);
void strdec(string_t *s);
int char_at(string_t *s, int idx);
string_view_t *view(string_t *s, unsigned int start, unsigned int end);
void view_done(string_view_t *view);
string_t *stringify(string_view_t *view);
void print_view(string_view_t *view, FILE *f, int nl);
string_t *substring(string_t *from, size_t start, size_t end);
int string_compare(string_t *a, string_t *b);
int starts_with(string_t *a, string_t *pat);
int ends_with(string_t *a, string_t *pat);
int rcontains(string_t *a, string_t *s);
int contains(string_t *a, string_t *sub);
string_t *concat(string_t *a, string_t *b);
string_t *trim(string_t *original);
string_tokens_t *tokenize(string_t *s, char delim);
void tokens_done(string_tokens_t *tokens);
string_t *get_token(string_tokens_t *toks, int idx);
string_t *readline(FILE *source);
char *charp(string_t *s) { return s->data; }

#endif