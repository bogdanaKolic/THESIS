/* Example code from the wiki page:
 * https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>

void handle_error (int retval)
{
     printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
     exit(1);
}

