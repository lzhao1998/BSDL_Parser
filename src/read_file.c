#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//check existing of file
char* check_file_exists(char *file_name){
  // F_OK use to test for existence of file.
  if( access( file_name, F_OK ) != -1 ) {
    return "File Exists";
  }
  else{
    return NULL;
  }
}

//read file

char* read_file(char *file_name){
  //FILE is an object type suitable for storing information for a file stream.
   char ch;
   FILE *file_read;

   // r = read mode, w = write mode
   file_read = fopen(file_name, "r");

   if (file_read == NULL){
     return NULL;
   }
   else{
     //just temporary set the size of the line
     char line [ 128 ];
     //read each line and display
     //fgets use to read the file until it meet '\n' , NULL or EOF
     while (fgets(line, sizeof(line), file_read) != NULL)
     {
       //display line
       fputs (line, stdout);
     }
     fclose (file_read); //close file
     return "Not empty";
   }
}
