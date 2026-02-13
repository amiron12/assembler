/*
First pass of a two-pass assembler
input: file.as

*/
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: second pass func will be declared here for encapsulation */

void start_pass(FILE *fp)
{
    char line[MAX_LINE_LENGTH];
    
    if(fgets(line, MAX_LINE_LENGTH, stdin) == NULL) /*  */
        {/* TODO: end of file */}

    


}





int line_pass(char *line)
{

}   