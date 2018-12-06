#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t *string(char*from);
string_t *view(string_t *s, unsigned int start, unsigned int end);
string_t *substring(char *from, unsigned int start, unsigned int end);
int charAt(string_t *s, int idx);
// int contains(string_t *s, string_t* pat);
// int contains(string_t *s, char c);
// int indexof(string_t *s, string_t *s);
// int indexof(string_t *s, char c);