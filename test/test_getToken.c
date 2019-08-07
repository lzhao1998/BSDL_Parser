#include <stdlib.h>
#include <errno.h>
#include "unity.h"
#include "Common.h"
#include "Token.h"
#include "Error.h"
#include "Tokenizer.h"
#include "Exception.h"
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {}

void test_skipWhiteSpaces_given_empty_string_expect_to_skip_till_end_of_string() {
  char *endPtr = skipWhiteSpaces("               ");

  TEST_ASSERT_EQUAL(0, *endPtr);
}

void test_skipWhiteSpaces_given____Hello_string_expect_to_skip_till_H() {
  char *endPtr = skipWhiteSpaces("   Hello   ");

  TEST_ASSERT_EQUAL('H', *endPtr);
}
//NULL
void test_getToken_given_NULL_expect_NULL_TOKEN() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("");
  token = getToken(tokenizer);

  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING(NULL, token->str);
  freeToken(token);
  freeTokenizer(tokenizer);
}
//IntegerToken
void test_getToken_given_123_expect_Integer_Token_123() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("123");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(123, intToken->value);
  TEST_ASSERT_EQUAL_STRING("123", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_space_123_expect_Integer_Token_123() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("   123");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(123, intToken->value);
  TEST_ASSERT_EQUAL_STRING("123", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_000_expect_Integer_Token_0() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("000");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(0, intToken->value);
  TEST_ASSERT_EQUAL_STRING("000", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_076_expect_Integer_Token_62() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("  076  ");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(62, intToken->value);
  TEST_ASSERT_EQUAL_STRING("076", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_038_expect_invalid_integer_exception_to_be_thrown() {
  CEXCEPTION_T ex;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("038");

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

void test_getToken_given_0x0_expect_Integer_Token_0() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0x0");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(0, intToken->value);
  TEST_ASSERT_EQUAL_STRING("0x0", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_123_space_expect_Integer_Token_123() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("123   ");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL(123, intToken->value);
  TEST_ASSERT_EQUAL_STRING("123", intToken->str);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0xface_expect_Integer_Token_0xface() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" 0xface ");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL_STRING("0xface", intToken->str);
  TEST_ASSERT_EQUAL(0xface, intToken->value);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0xFae_expect_Integer_Token_0xfae() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" 0xFae ");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL_STRING("0xFae", intToken->str);
  TEST_ASSERT_EQUAL(0xfae, intToken->value);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0xABCD_expect_Integer_Token_0xabcd() {
  Token *token;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" 0xABCD ");
  token = getToken(tokenizer);
  intToken = (IntegerToken *)token;

  TEST_ASSERT_NOT_NULL(intToken);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, intToken->type);
  TEST_ASSERT_EQUAL_STRING("0xABCD", intToken->str);
  TEST_ASSERT_EQUAL(0xabcd, intToken->value);
  freeToken(intToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0xfaz_expect_return_ERR_INVALID_TOKEN() {
  CEXCEPTION_T ex;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0xfaz");

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

void test_getToken_given_0x12za_expect_return_ERR_INVALID_TOKEN() {
  CEXCEPTION_T ex;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0x12za");

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

void test_getToken_given_0z12_expect_return_ERR_INVALID_TOKEN() {
  CEXCEPTION_T ex;
  IntegerToken *intToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0z12");

  Try{
    intToken = (IntegerToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

//Float
void test_getToken_given_1dot23_expect_Float_Token_1dot23() {
  Token *token;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("1.23  ");
  token = getToken(tokenizer);
  floatToken = (FloatToken *)token;

  TEST_ASSERT_NOT_NULL(floatToken);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, floatToken->type);
  TEST_ASSERT_EQUAL(1.23, floatToken->value);
  TEST_ASSERT_EQUAL_STRING("1.23", floatToken->str);
  freeToken(floatToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_1dot_expect_Float_Token_1() {
  Token *token;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("  1.");
  token = getToken(tokenizer);
  floatToken = (FloatToken *)token;

  TEST_ASSERT_NOT_NULL(floatToken);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, floatToken->type);
  TEST_ASSERT_EQUAL(1.000, floatToken->value);
  TEST_ASSERT_EQUAL_STRING("1.", floatToken->str);
  freeToken(floatToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot123_expect_Float_Token_0dot123() {
  Token *token;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.123");
  token = getToken(tokenizer);
  floatToken = (FloatToken *)token;

  TEST_ASSERT_NOT_NULL(floatToken);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, floatToken->type);
  TEST_ASSERT_EQUAL(0.123, floatToken->value);
  TEST_ASSERT_EQUAL_STRING("0.123", floatToken->str);
  freeToken(floatToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_space_0dot123_space_1_expect_Float_Token_0dot123() {
  Token *token;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("   0.123  1");
  token = getToken(tokenizer);
  floatToken = (FloatToken *)token;

  TEST_ASSERT_NOT_NULL(floatToken);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, floatToken->type);
  TEST_ASSERT_EQUAL(0.123, floatToken->value);
  TEST_ASSERT_EQUAL_STRING("0.123", floatToken->str);
  freeToken(floatToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot123E_plus_4_expect_Float_Token_1230() {
  Token *token;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.123e+4");
  token = getToken(tokenizer);
  floatToken = (FloatToken *)token;

  TEST_ASSERT_NOT_NULL(floatToken);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, floatToken->type);
  TEST_ASSERT_EQUAL(1230, floatToken->value);
  TEST_ASSERT_EQUAL_STRING("0.123e+4", floatToken->str);
  freeToken(floatToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_4e_expect_return_ERR_INVALID_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("4e");

  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_123hello_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("123hello");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot444ee_plus_4_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.444ee+4");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot567e_plus4e_minus7_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.567e+4e-7");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot567e_plus4dot7_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.567e+4.7");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot887e_plusminus7_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.887e+-4");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_0dot12dot3e_minus4_expect_throw_ERR_INVALID_FLOAT_TOKEN() {
  CEXCEPTION_T ex;
  FloatToken *floatToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("0.12.3e-4");
  Try{
    floatToken = (FloatToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_FLOAT, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

 //STRING
 void test_getToken_given_string_empty_expect_String_Token_empty() {
   Token *token;
   StringToken *strToken;
   Tokenizer *tokenizer;

   tokenizer = initTokenizer("\"\"");
   token = getToken(tokenizer);
   strToken = (StringToken *)token;

   TEST_ASSERT_NOT_NULL(strToken);
   TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
   TEST_ASSERT_EQUAL_STRING("", strToken->str);
   TEST_ASSERT_EQUAL(2, tokenizer->index);
   freeToken(strToken);
   freeTokenizer(tokenizer);
 }

void test_getToken_given_string_hello_expect_String_Token_hello() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"hello\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello", strToken->str);
  TEST_ASSERT_EQUAL(7, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_space_hello_expect_String_Token_hello() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("    \"hello\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello", strToken->str);
  TEST_ASSERT_EQUAL(11, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_space_expect_String_Token_hello() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"hello\"    ");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello", strToken->str);
  TEST_ASSERT_EQUAL(7, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_space_world_expect_String_Token_hello_world() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"hello  world\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello  world", strToken->str);
  TEST_ASSERT_EQUAL(14, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello123_expect_String_Token_hello123() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"hello123\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL(8, strToken->length);
  TEST_ASSERT_EQUAL_STRING("hello123", strToken->str);
  TEST_ASSERT_EQUAL(10, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_123hello_expect_String_Token_hello123() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"123hello\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("123hello", strToken->str);
  TEST_ASSERT_EQUAL(10, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_1_expect_String_Token_1() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"1\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("1", strToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_a_expect_String_Token_a() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"a\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("a", strToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_symbol_expect_String_Token_symbol() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\",./?,.\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING(",./?,.", strToken->str);
  TEST_ASSERT_EQUAL(8, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_without_closing_ampersand_sign_expect_return_INVALID_STRING_TYPE() {
  CEXCEPTION_T ex;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"hello");

  Try{
    strToken = (StringToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_a_expect_String_Token_symbol() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\"a\"");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("a", strToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_using_quotation_expect_String_Token_hello() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'hello\'");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello", strToken->str);
  TEST_ASSERT_EQUAL(7, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_space_using_quotation_expect_String_Token_hello() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'hello\'    ");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL(5, strToken->length);
  TEST_ASSERT_EQUAL_STRING("hello", strToken->str);
  TEST_ASSERT_EQUAL(7, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_world_using_quotation_expect_String_Token_hello_world() {
  Token *token;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'hello world\'");
  token = getToken(tokenizer);
  strToken = (StringToken *)token;

  TEST_ASSERT_NOT_NULL(strToken);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, strToken->type);
  TEST_ASSERT_EQUAL_STRING("hello world", strToken->str);
  TEST_ASSERT_EQUAL(13, tokenizer->index);
  freeToken(strToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_string_hello_without_closing_quotation_sign_expect_return_INVALID_STRING_TYPE() {
  CEXCEPTION_T ex;
  StringToken *strToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'hello");

  Try{
    strToken = (StringToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}
//Char
void test_getToken_given_char_A_space_expect_Char_Token_A() {
  Token *token;
  CharConstToken *charToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'A\'");
  token = getToken(tokenizer);
  charToken = (CharConstToken *)token;

  TEST_ASSERT_NOT_NULL(charToken);
  TEST_ASSERT_EQUAL(TOKEN_CHAR_TYPE, charToken->type);
  TEST_ASSERT_EQUAL_STRING("A", charToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(charToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_char_d_space_expect_Char_Token_d() {
  Token *token;
  CharConstToken *charToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'d\'");
  token = getToken(tokenizer);
  charToken = (CharConstToken *)token;

  TEST_ASSERT_NOT_NULL(charToken);
  TEST_ASSERT_EQUAL(TOKEN_CHAR_TYPE, charToken->type);
  TEST_ASSERT_EQUAL(1, charToken->length);
  TEST_ASSERT_EQUAL_STRING("d", charToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(charToken);
  freeTokenizer(tokenizer);
}

//should be string token or char token?
void test_getToken_given_empty_char_expect_Char_Token() {
  CEXCEPTION_T ex;
  CharConstToken *charToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'\'");

  Try{
    charToken = (CharConstToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_CHAR, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_char_symbol_expect_return_INVALID_CHAR_TYPE() {
  CEXCEPTION_T ex;
  CharConstToken *charToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'$\'");

  Try{
    charToken = (CharConstToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_CHAR, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

void test_getToken_given_char_number_expect_return_INVALID_CHAR_TYPE() {
  CEXCEPTION_T ex;
  CharConstToken *charToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("\'1\'");

  Try{
    charToken = (CharConstToken *)getToken(tokenizer);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_CHAR, ex->errorCode);
  }
  freeToken(ex->data);
  freeTokenizer(tokenizer);
}

//Identifier
void test_getToken_given_identifier_apple_expect_Identifier_Token_apple() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("apple");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("apple", IdToken->str);
  TEST_ASSERT_EQUAL(5, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_value2_expect_Identifier_Token_value2() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("value2");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("value2", IdToken->str);
  TEST_ASSERT_EQUAL(6, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier__hp_expect_Identifier_Token__hp() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_hp");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_hp", IdToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier__hp_omen_expect_Identifier_Token__hp_omen() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_hp_omen");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_hp_omen", IdToken->str);
  TEST_ASSERT_EQUAL(8, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_h_expect_Identifier_Token_h() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("h");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("h", IdToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_underscore_expect_return_Identifier_Token() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_", IdToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_E_expect_Identifier_Token_E() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("E");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("E", IdToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier__123_expect_Identifier_Token__123() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_123");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_123", IdToken->str);
  TEST_ASSERT_EQUAL(4, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_k12_expect_Identifier_Token_k12() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("k12");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("k12", IdToken->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier__Omg123__expect_Identifier_Token__Omg123_() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_Omg123_");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL(8, IdToken->length);
  TEST_ASSERT_EQUAL_STRING("_Omg123_", IdToken->str);
  TEST_ASSERT_EQUAL(8, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_hello_world_expect_Identifier_Token_hello_world() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("hello_world");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("hello_world", IdToken->str);
  TEST_ASSERT_EQUAL(11, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_space_hello_world_expect_Identifier_Token_hello_world() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("    hello_world");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("hello_world", IdToken->str);
  TEST_ASSERT_EQUAL(15, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_hello_world_space_expect_Identifier_Token_hello_world() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("hello_world    ");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("hello_world", IdToken->str);
  TEST_ASSERT_EQUAL(11, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_zeez_dot_abc_expect_return__zeez() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_zeez.abc");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_zeez", IdToken->str);
  TEST_ASSERT_EQUAL(5, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_identifier_zeez_symbol_expect_return_zeez() {
  Token *token;
  IdentifierToken *IdToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("_zeez#");
  token = getToken(tokenizer);
  IdToken = (IdentifierToken *)token;

  TEST_ASSERT_NOT_NULL(IdToken);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, IdToken->type);
  TEST_ASSERT_EQUAL_STRING("_zeez", IdToken->str);
  TEST_ASSERT_EQUAL(5, tokenizer->index);
  freeToken(IdToken);
  freeTokenizer(tokenizer);
}

//OPERATOR
void test_getToken_given_operator_minus_expect_Operator_Token_minus() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("-");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("-", OpToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_operator_minus_plus_expect_Operator_Token_minus_plus() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("-+");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("-", OpToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_operator_and_expect_Operator_Token_ampersand() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("&&");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("&", OpToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_space_power_and_expect_Operator_Token_power() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" ^");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("^", OpToken->str);
  TEST_ASSERT_EQUAL(2, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_operator_bracket_expect_Operator_Token_openBracket() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("()");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("(", OpToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_operator_space_dot123_expect_Operator_Token_dot() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" .123");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING(".", OpToken->str);
  TEST_ASSERT_EQUAL(2, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

void test_getToken_given_operator_symbol_expect_Operator_Token_slash() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer(" /*-+!@#$");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("/", OpToken->str);
  TEST_ASSERT_EQUAL(2, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}


void test_getToken_given_operator_or_abc_expect_Operator_Token_and() {
  Token *token;
  OperatorToken *OpToken;
  Tokenizer *tokenizer;

  tokenizer = initTokenizer("||abc");
  token = getToken(tokenizer);
  OpToken = (OperatorToken *)token;

  TEST_ASSERT_NOT_NULL(OpToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, OpToken->type);
  TEST_ASSERT_EQUAL_STRING("|", OpToken->str);
  TEST_ASSERT_EQUAL(1, tokenizer->index);
  freeToken(OpToken);
  freeTokenizer(tokenizer);
}

//test multiple token in 1 shot
void test_getToken_given_string_and_identifier_expect_String_and_Identifier_token() {
  Token *token;
  Tokenizer *tokenizer;
  tokenizer = initTokenizer("\"hello\" hello");

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_STRING_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING("hello", token->str);
  TEST_ASSERT_EQUAL(7, tokenizer->index);
  freeToken(token);

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING("hello", token->str);
  TEST_ASSERT_EQUAL(13, tokenizer->index);
  freeToken(token);

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING(NULL, token->str);
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_getToken_given_char_float_hex_expect_Char_Float_Integer_token() {
  Token *token;
  Tokenizer *tokenizer;
  IntegerToken *intToken;
  FloatToken *floatToken;
  tokenizer = initTokenizer("\'h\' 0.213  0xFF");

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_CHAR_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING("h", token->str);
  TEST_ASSERT_EQUAL(3, tokenizer->index);
  freeToken(token);

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_FLOAT_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING("0.213", token->str);
  TEST_ASSERT_EQUAL(9, tokenizer->index);
  freeToken(token);

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING("0xFF", token->str);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, token->type);
  TEST_ASSERT_EQUAL(15, tokenizer->index);
  freeToken(token);

  token = getToken(tokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE, token->type);
  TEST_ASSERT_EQUAL_STRING(NULL, token->str);
  freeToken(token);
  freeTokenizer(tokenizer);
}

//test for 'checkFor0x' function
void test_gv_0x12_return_1()
{
  char *temp = "0x12";
  int i = checkFor0x(temp);

  TEST_ASSERT_EQUAL(1, i);
}

void test_gv_0x_return_1()
{
  char *temp = "0x";
  int i = checkFor0x(temp);

  TEST_ASSERT_EQUAL(1, i);
}

void test_gv_0X12_return_0()
{
  char *temp = "0X12";
  int i = checkFor0x(temp);

  TEST_ASSERT_EQUAL(0, i);
}

void test_gv_0z12_return_0()
{
  char *temp = "0z12";
  int i = checkFor0x(temp);

  TEST_ASSERT_EQUAL(0, i);
}

void test_gv_12_dot_3_return_0()
{
  char *temp = "12.3";
  int i = checkFor0x(temp);

  TEST_ASSERT_EQUAL(0, i);
}

//test for 'checkForHexDigit' function
void test_given_1234_expect_return_1()
{
  char *str = "1234";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_123ABC_expect_return_1()
{
  char *str = "123ABC";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_123Abc_expect_return_1()
{
  char *str = "123Abc";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_123ABCEF_expect_return_1()
{
  char *str = "123ABCEF";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_123EFG_expect_return_0()
{
  char *str = "123EFG";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(0, i);
}

void test_given_0x23_expect_return_0()
{
  char *str = "0x23";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(0, i);
}


void test_given_123abc_expect_return_1()
{
  char *str = "123abc";
  int i = checkForHexDigit(str);

  TEST_ASSERT_EQUAL(1, i);
}

//test for 'checkforInt' function
void test_given_123_expect_return_1()
{
  char *str = "123";
  int i = checkforInt(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_123dot4_expect_return_0()
{
  char *str = "123.4";
  int i = checkforInt(str);

  TEST_ASSERT_EQUAL(0, i);
}

void test_given_123abcd_expect_return_0()
{
  char *str = "123abcd";
  int i = checkforInt(str);

  TEST_ASSERT_EQUAL(0, i);
}

void test_given_123null_expect_return_1()
{
  char *str = "123\0";
  int i = checkforInt(str);

  TEST_ASSERT_EQUAL(1, i);
}

//Test check value is over 8 or not
void test_given_0123_expect_return_1()
{
  char *str = "0123";
  int i = checkForOctal(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_0567_expect_return_1()
{
  char *str = "0567";
  int i = checkForOctal(str);

  TEST_ASSERT_EQUAL(1, i);
}

void test_given_08_expect_return_0()
{
  char *str = "08";
  int i = checkForOctal(str);

  TEST_ASSERT_EQUAL(0, i);
}
