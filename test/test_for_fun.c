#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <strings.h>
#include "unity.h"
#include "Common.h"
#include "Token.h"
#include "Error.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "handlePortDescription.h"


/*
extern int errno ;

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
/*----------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------*/
/*
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
}*/
/*----------------------------------------------------------------------*/
/*
char *naming[] = {"aasda","bb","cc","dd","ee"};
char *nothing[] = {};

void test_str_array(void){
  char z = '-';
  int i = sizeof(naming)/sizeof(char*);
  printf("i size is %d\n",i);
  int j = sizeof(nothing)/sizeof(char*);
  printf("i size is %d\n",j);
  if (z == 45){ // 45 and "-"
    printf("yes\n");
  }else{
    printf("no\n");
  }

  TEST_ASSERT_NULL(NULL);
}

void test_compareDesName(void){
  int i;
  i = compareDescriptionName("entity");
  printf("entity is :%d\n",i);
  i = compareDescriptionName("port");
  printf("port is %d\n", i);
  i = compareDescriptionName("asd");
  printf("asd is %d\n", i);
}

void test_strcmp_case_insensitive(void){
  char *str1 = "hel_lo";
  char *str2 = "Hel_LO";

  int i = strcasecmp(str1,str2);
  printf("strcmp answer is :%d\n",i);
}*/
/*----------------------------------------------------------------------*/
/*
void test_linked_list(void){
  LinkedList *port;
  port = listInit();

  portDesc *des;
  des =  (portDesc*)malloc(sizeof(portDesc));
  des->portName = "abc";
  des->pinType = 1;
  des->bitType = 1;
  des->integer1 = 5;
  des->integer2 = 5;
  des->upDown = 1;

  Item *item;
  item = initItem(des);
  listAdd(port,item);

  TEST_ASSERT_NULL(NULL);
  portDesc *temp;
  temp = (portDesc*)item ->data;
  printf("item is %d\n",temp->pinType );
  printf("item2 is %d\n",((portDesc*)(port->head->data))->pinType );
  // TEST_ASSERT_EQUAL_STRING("abc",des->portName);
  // TEST_ASSERT_EQUAL(1,port->len);
  // TEST_ASSERT_EQUAL_PTR(des,port->head->data);
   TEST_ASSERT_EQUAL(((portDesc*)(port->head->data))->pinType,1);
}*/

/*----------------------------------------------------------------------*/
/*
void test_sizeof_str_array(void){
  char str[600];
  strcpy(str,"");

  printf("strlen is :%d\n",strlen(str));
  char *a = "hello";
  strcat(str,a);
  printf("strlen is :%d\n",strlen(str));
  a = "world";
  strcat(str,a);
  printf("strlen is :%d\n",strlen(str));
}*/
/*----------------------------------------------------------------------*/
/*
void test_check_after_malloc_is_null_or_not(void){
  Tokenizer *tokenizer;
  tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));
  printf("token->str is :%s\n", tokenizer->str);
  printf("token->str is :%d\n", tokenizer->index);
  printf("token->str length is :%d\n", strlen(tokenizer->str));
  if(strlen(tokenizer->str) == 0){
    printf("is null\n" );
  }
  TEST_ASSERT_NULL(NULL);
}*/

/*-----------------------------------------------------------------------*/
/*
void test_strcat(void){
  Token *token;
  Tokenizer *tokenizer;
  char str1[500];
  char str2[50];

  tokenizer = initTokenizer("hello world");
  token = getToken(tokenizer);

  strcat(str1,token->str);
  printf("str1 is %s\n", str1);
  freeToken(token);
  token = getToken(tokenizer);
  printf("str1 is %s\n", str1);

  freeToken(token);
  freeTokenizer(tokenizer);
}

char *strArr[] = {"hello","hi","hey",NULL};

int getint(char *strar[]){
  int i = 0;
  while(strArr[i] != NULL){
    i++;
    if(i == 100){
      break;
    }
  }
  return i;

}

void test_getstrArr(void){
  int i = getint(strArr);

  printf("i is %i\n", i);
}*/
/*-------------------------------------------------------*/
/*int *getINTARR(void){
  int *temp = (int*)malloc(sizeof(int) * 3);
  temp[0] = 2;
  temp[1] = 3;
  temp[2] = 5;

  if(temp[0] < temp[2]){
    temp[0] = 6;
  }

  return temp;
}


void test_getInt_array(void){
  int a,b,c;

  int *arr = getINTARR();
  a = arr[0];
  b = arr[1];
  c = arr[2];
  printf("a:%d b:%d c:%d\n",a,b,c);
}*/

void test_getToken_given_0xfaz_expect_return_ERR_INVALID_TOKEN() {
  CEXCEPTION_T ex;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.42e-4.7");

  Try{
    intToken = (IntegerToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_INTEGER, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_punct(void){

  printf("%d\n", ispunct(97));
}

/*
for VHDL identifier:
-need follow its rules pg217/444 (pg195)

description:
entity,generic,port,use,attribute,end

attribute:
COMPONENT_CONFORMANCE,PIN_MAP,TAP_SCAN_CLOCK,TAP_SCAN_IN,TAP_SCAN_MODE,TAP_SCAN_OUT,TAP_SCAN_RESET,COMPLIANCE_PATTERNS,INSTRUCTION_LENGTH,INSTRUCTION_OPCODE,
INSTRUCTION_CAPTURE,IDCODE_REGISTER,REGISTER_ACCESS,BOUNDARY_LENGTH,BOUNDARY_REGISTER,DESIGN_WARNING
*/
