#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//check existing of file
char* check_file_exists(char *file_name){
  if( access( file_name, F_OK ) != -1 ) {
    return "File Exists";
  }
  else{
    return NULL;
  }
}

//read file
/*
char* file_read(char *file_name){
  //FILE is an object type suitable for storing information for a file stream.
   char ch;
   FILE *file_read;

   // r = read mode, w = write mode
   file_read = fopen(file_name, "r");

   if (file_read == NULL){
     return NULL;
   }
   else{
     return file_read;
   }
}*/
