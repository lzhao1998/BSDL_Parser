#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "unity.h"
#include "Common.h"
#include "Token.h"
#include "Error.h"
#include "Tokenizer.h"
#include "Exception.h"

extern int errno ;
/*
void test_errno (void) {

   FILE * pf;
   int errnum;

   pf = fopen ("C:\\ZheHao\\hey.rar", "r");

   if (pf == NULL) {

      errnum = errno;
      fprintf(stderr, "Value of errno: %d\n", errno);
      perror("Error printed by perror");
      fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
   } else {
      printf("the file is not NULL\n");
      fclose (pf);
   }
}

void test_tokenizer(void){
  char *str = "hello world";
  Tokenizer *tokenizer;
  Token *token;

  tokenizer = initTokenizer(str);
  token = getToken(tokenizer);
  printf("token str 1 is :%s\n",token->str);
  freeToken(token);
  token = getToken(tokenizer);
  printf("token str 1 is :%s\n",token->str);
  freeToken(token);
  token = getToken(tokenizer);
  printf("token type is :%d\n",token->type);
  freeToken(token);
  freeTokenizer(tokenizer);
}*/
/*
void test_fopen_and_read_it(void){
  FILE *fp;
  char line[4096];
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  fp = fopen(filename, "r");
  int i =0;
  while(fgets(line,sizeof(line),fp) != NULL){
    i++;
    printf("file is :%s",line);
  }
  printf("pass\n");
}*/
/*
void test_fopen_and_read_line_by_line(void){
  FILE *fp;
  char line[512];
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  fp = fopen(filename, "r");
  fgets(line,sizeof(line),fp);
  printf("file is :%s\n",line);
  fgets(line,sizeof(line),fp);
  printf("file is :%s\n",line);

  printf("pass\n");
}*/
/*
void test_tokenizer_when_input_is_empty_string_and_NULL(void){
  Tokenizer *tokenizer;
  Token *token;

  tokenizer = initTokenizer(NULL);
  if (tokenizer->str == NULL){
    printf("str1 is NULL \n");
  }else{
    printf("str1 is :%s\n",tokenizer->str);
  }
  freeTokenizer(tokenizer);

  tokenizer = initTokenizer("");
  if (tokenizer->str == NULL){
    printf("str2 is NULL \n");
  }else{
    printf("str2 is :%s\n",tokenizer->str);
  }
  freeTokenizer(tokenizer);
}*/

void test_get_line_length_for_fgets(void){
  FILE *fp;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_for_fun_use.txt";
  char line[4096];
  fp = fopen(filename, "r");
  fgets(line,sizeof(line),fp);
  printf("line is :%s\n",line);
  printf("length is :%d\n",strlen(line));
  char temp = line[0];
  printf("temp num is :%d\n",temp);
  if(temp == 10){  //REFER TO ASCII TABLE , newline is 10 for decimal, null is 0;
    printf("is newline\n");
  }
  char *tem[strlen(line)+1];
  strncpy(tem,line,16);
  printf("tem is :%s\n",tem);

  fclose(fp);
}
