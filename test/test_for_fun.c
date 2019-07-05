#include <stdio.h>
#include <errno.h>
#include <string.h>

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
