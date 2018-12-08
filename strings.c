#include "strings.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TEST
#include <assert.h>
#include <time.h>
#endif

int isWhitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v';
}

char *sha256_hexdigest(char *o) { return strdup("1sd09e09fa090ae7d"); }

string_t *string(char *from) {
  string_t *s = malloc(sizeof(*s));
  s->data = strdup(from);
  s->hash = sha256_hexdigest(from);
  s->size = strlen(from);
  s->refcnt = 1;
  return s;
}

void strinc(string_t *s) { s->refcnt++; }

void strdec(string_t *s) {
  s->refcnt--;
  if (!s->refcnt) {
    free(s->data);
    free(s->hash);
    free(s);
  }
}

string_view_t *view(string_t *s, unsigned int start, unsigned int end) {
  string_view_t *view = malloc(sizeof(*view));
  view->s = s;
  strinc(s);
  view->start = start;
  view->end = end;
  return view;
}

string_t *substring(string_t *from, size_t start, size_t end) {
  char *d = from->data;
  if (start >= from->size || end > from->size || start < 0 || end < 0 ||
      start > end) {
    return NULL;
  }
  d += start;
  char *last = from->data + end;
  char prev = *last;
  *last = '\0';
  string_t *n = string(d);
  *last = prev;
  return n;
}

int char_at(string_t *s, int idx) {
  if (s->size <= idx) {
    return -1;
  }
  return (int)*(s->data + idx);
}

void view_done(string_view_t *view) {
  strdec(view->s);
  free(view);
}

int string_compare(string_t *a, string_t *b) {
  return strcmp(a->data, b->data);
}

void print_view(string_view_t *view, FILE *f, int nl) {
  for (int i = view->start; i < view->end; i++) {
    fputc(*(view->s->data + i), f);
  }
  if (nl) {
    fputc(10, f);
  }
}

string_t *stringify(string_view_t *view) {
  char *v = view->s->data + view->start;
  char *e = view->s->data + view->end;
  char p = *e;
  *e = '\0';
  string_t *ret = string(v);
  *e = p;
  return ret;
}

string_t *concat(string_t *a, string_t *b) {
  char *n = malloc((sizeof(char)) * (a->size + b->size));
  strncpy(n, a->data, a->size);
  n += a->size;
  strncpy(n, b->data, b->size);
  n -= a->size;
  string_t *ret = malloc(sizeof(*ret));
  ret->data = n;
  ret->refcnt = 1;
  ret->hash = sha256_hexdigest(n);
  ret->size = a->size + b->size;
  return ret;
}

int starts_with(string_t *a, string_t *pat) {
  if (pat->size > a->size) {
    return 0;
  }
  for (int i = 0; i < pat->size; i++) {
    if (*(a->data + i) != *(pat->data + i)) {
      return 0;
    }
  }
  return 1;
}

int ends_with(string_t *a, string_t *pat) {
  if (pat->size > a->size) {
    return 0;
  }
  char *start = a->data + a->size - pat->size;
  for (int i = 0; i < pat->size; i++) {
    if (*(start + i) != *(pat->data + i)) {
      return 0;
    }
  }
  return 1;
}

string_tokens_t *tokenize(string_t *s, char delim) {
  string_tokens_t *list = NULL;
  string_tokens_t *ret = NULL;
  int start = 0;
  int idx = 0;
  while (idx < s->size) {
    while (*(s->data + idx) != delim && idx < s->size) {
      idx++;
    }
    string_t *tok = substring(s, start, idx);
    idx++;       // discard delim
    start = idx; // begin substrings from idx
    strinc(tok);
    if (list == NULL) {
      list = calloc(1, sizeof(*list));
      list->token = tok;
      ret = list;
      ret->size++;
    } else {
      list->next = calloc(1, sizeof(*list));
      list = list->next;
      list->token = tok;
      ret->size++;
    }
    strdec(tok);
  }
  return ret;
}

int rcontains_help(string_t *a, string_t *s, int ai, int si) {
  if (si == s->size) {
    return 1;
  }
  if (ai == a->size) {
    return 0;
  }
  if (char_at(a, ai) == char_at(s, si)) {
    return rcontains_help(a, s, ai + 1, si + 1);
  } else {
    return rcontains_help(a, s, ai + 1, si);
  }
}

int rcontains(string_t *a, string_t *s) { return rcontains_help(a, s, 0, 0); }

int contains(string_t *a, string_t *sub) {
  if (sub->size > a->size) {
    return 0;
  }
  if (!string_compare(a, sub)) {
    return 1;
  }

  int ai = 0;
  int si = 0;

  while (ai < a->size) {
    if (char_at(a, ai) != char_at(sub, si)) {
      ai++;
    }
    while (si < sub->size && char_at(a, ai) == char_at(sub, si)) {
      ai++;
      si++;
    }
    if (si == sub->size && char_at(sub, si - 1) == char_at(a, ai - 1)) {
      return 1;
    } else {
      si = 0;
    }
  }
  return 0;
}

string_t *trim(string_t *original) {
  size_t start, end;
  size_t idx = 0;
  while (idx < original->size - 1 && isWhitespace(*(original->data + idx))) {
    idx++;
  }
  start = idx;
  idx = original->size - 1;
  while (idx > start && isWhitespace(*(original->data + idx))) {
    idx--;
  }
  end = idx;

  return substring(original, start, end);
}

string_t *get_token(string_tokens_t *toks, int idx) {
  if (idx >= toks->size) {
    return NULL;
  }
  string_tokens_t *ptr = toks;
  while (idx--) {
    ptr = ptr->next;
  }
  return ptr->token;
}

#ifdef TEST
int random_bound() {
  srand(time(NULL));
  return rand() % 75000;
}
#elif defined(DEBUG)
int random_bound() { return 500; }
#else

int random_bound() { return 1; }

#endif

string_t *readline(FILE *source) {
  int size = 10;
  char *buffer = NULL;
  int c;
  int idx = 0;
  c = getc(source);
  while (c != '\n' && c != EOF) {
    if (buffer == NULL) {
      buffer = malloc((size_t)size);
    }
    *(buffer + idx) = (char)c;
    idx++;
    if (idx == size) {
      size = (int)(size * 1.5);
      if (size < 0) {
        return NULL;
      }
      buffer = realloc(buffer, (size_t)size);
      if (buffer == NULL) {
        return NULL;
      }
    }
    c = getc(source);
  }
  *(buffer + idx + 1) = '\0';
  return buffer ? string(buffer) : NULL;
}

void tokens_done(string_tokens_t *tokens) {
  if (tokens == NULL) {
    return;
  } else {
    tokens_done(tokens->next);
    free(tokens->next);
    strdec(tokens->token);
  }
}

char *charp(string_t *s) { return s->data; }

int main(int argc, char const **argv) {
  // for (int i = 0; i < random_bound(); i++) {
  //  string_t* s = string("Hello world!   ");
  //  string_t* r = string("   hi there! ");
  //  string_t* h = substring(s, 0, 5);
  //  string_t* g = string("Where in the world is carmen sandiego?");

  FILE *f = fopen("/home/thomas/Desktop/cstring/colors.csv", "r");
  if (f == NULL) {
    strerror(errno);
    abort();
  }
  string_t *t;
  while ((t = readline(f)) != NULL) {
    string_tokens_t *tokens = tokenize(t, ',');
    for (int i = 0; i < tokens->size; i++) {
      printf("t: %s |", charp(get_token(tokens, i)));
    }
    strdec(t);
    tokens_done(tokens);
  }

  // string_t* w = string("       ");
  // string_t* world1 = substring(g, 13, 18);
  // string_t* world2 = substring(s, 6, 11);
  // int eq = string_compare(world1, world2);
  // string_view_t* v1 = view(s, 0, 5);
  // string_view_t* v2 = view(s, 6, 11);
  // string_view_t* v3 = view(s, 5, 7);

  // string_t* c1 = concat(s, g);
  // string_t* c2 = concat(world1, world2);

  // int n1 = rcontains(s, h);
  // assert(n1 == 1);
  // int n2 = rcontains(s, world1);
  // assert(n2 == 1);
  // int n3 = rcontains(s, world2);
  // assert(n3 == 1);
  // int n4 = rcontains(s, g);
  // assert(n4 == 0);
  // int n5 = rcontains(s, w);
  // assert(n5 == 0);
  // int n6 = rcontains(g, world2);
  // assert(n6 == 1);
  // int n7 = rcontains(world2, g);
  // assert(n7 == 0);

  // string_t* t1 = trim(w);
  // string_t* t2 = trim(s);
  // string_t* t3 = trim(r);

  // int y1 = starts_with(s, h);
  // int n8 = starts_with(s, world1);

  // // print_view(v1, stdout, 1);
  // // print_view(v2, stdout, 1);
  // // print_view(v3, stdout, 1);

  // string_t* s1 = stringify(v1);
  // string_t* s2 = stringify(v2);
  // string_t* s3 = stringify(v3);

  // view_done(v1);
  // view_done(v2);
  // view_done(v3);

  // char x = (char) char_at(h, 2);
  // strdec(s);
  // strdec(h);
  // strdec(r);
  // strdec(w);
  // strdec(g);
  // strdec(world1);
  // strdec(world2);
  // strdec(s1);
  // strdec(s2);
  // strdec(s3);
  // strdec(c1);
  // strdec(c2);
  // strdec(t1);
  // strdec(t2);
  // strdec(t3);
  // }
}