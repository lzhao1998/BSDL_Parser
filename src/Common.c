#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "Common.h"

// skip the space
char *skipWhiteSpaces(char *str) {
  while(*str == ' ' || *str == '\t') str++;
  return str;
}

// skip the all things except space
char *skipNonWhiteSpaces(char *str) {
  while(*str != ' ' && *str != '\t' && *str != '\0') str++;
  return str;
}

// skip alphabet and number expect space and symbol
char *skipAlphanumerals(char *str) {
  while(isalpha(*str) || isdigit(*str)) str++;
  return str;
}

char *createSubstring(char *str, int start, int len) {
  char *newStr = malloc(len + 1);

  //printf("%s (%d, %d)\n", str, start, len);

  memcpy(newStr, &str[start], len);
  newStr[len] = 0;
  return newStr;
}
