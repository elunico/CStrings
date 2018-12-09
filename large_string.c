#include <stdio.h>
#include "strings.h"

int s_main(int argc, char const *argv[])
{
  // MY STRING LIBRARY OVER NATIVE C 
  string_t *s = string("Hello");
  string_t *n = string(charp(s));
  for (int i = 0; i < 2500; i++) {
  	string_t *old = n;
    n = concat(old, s);
    strdec(old);
  }
  printf("strlen: %ld, size: %ld\n", strlen(charp(n)), n->size);
  strdec(s);

  string_t *a = substring(n, 1,10);
  printf("Done first substring\n");
  fflush(stdout);
  string_t *b = substring(n, 1, n->size);
  printf("Done second substring\n");
  fflush(stdout);
  string_t *c = substring(n, 1, n->size);
  printf("Done third substring\n");
  fflush(stdout);
  string_t *d = substring(n, 0, n->size -1);
  printf("Done fourth substring\n");
  fflush(stdout);
  string_t *e = substring(n, 1, n->size -1);
  printf("Done fifth substring\n");
  fflush(stdout);

  string_view_t *a1 = view(n, 1,10);
  printf("Done first view\n");
  fflush(stdout);
  string_view_t *b1 = view(n, 1, n->size);
  printf("Done second view\n");
  fflush(stdout);
  string_view_t *c1 = view(n, 1, n->size);
  printf("Done third view\n");
  fflush(stdout);
  string_view_t *d1 = view(n, 0, n->size -1);
  printf("Done fourth view\n");
  fflush(stdout);
  string_view_t *e1 = view(n, 1, n->size -1);
  printf("Done fifth view\n");
  fflush(stdout);

  strdec(n);

  strdec(a);
  strdec(b);
  strdec(c);
  strdec(d);
  strdec(e);

  view_done(a1);
  view_done(b1);
  view_done(c1);
  view_done(d1);
  view_done(e1);
// NATIVE C IMPLEMENTATION
	// char *s = "hello";
 //  char *n = strdup(s);
 //  for (int i =0 ; i < 1000; i++) {
 //    char *old = n; 
 //    int l1 = strlen(old);
 //    int l2 = strlen(s);
 //    n = malloc(l1 + l2 + 1);
 //    strcpy(n, old);
 //    n += l1;
 //    strcpy(n, s);
 //    n -= l1;
 //    free(old);
 //  }
 //  free(n);
  return 0;
}