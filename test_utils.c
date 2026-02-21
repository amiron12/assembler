#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




void print_symbol_table(symbol *head) {
    symbol *temp = head;
    printf("\n--- DEBUG: Symbol Table ---\n");
    /* Column headers for clarity */
    printf("%-20s | %-8s | %-15s\n", "Label Name", "Address", "Attribute");
    printf("----------------------------------------------------------\n");

    while (temp != NULL) {
        /* Corrected: 2 placeholders (%s, %d) for the first 2 variables */
        printf("%-20s | %-8d | ", temp->name, temp->address);
        
        /* Handling the attribute enum separately to avoid the format warning */
        switch(temp->atr) {
            case code:     printf("code");     break;
            case data:     printf("data");     break;
            case external: printf("external"); break;
            case entry:    printf("entry");    break;
            default:       printf("unknown");  break;
        }
        
        printf("\n");
        temp = temp->next;
    }
}
void print_machine_images() {
    int i;

    printf("\n--- DEBUG: Code Image (Instructions) ---\n");
    /* Starts at 100. Prints only the words added so far. */
    for (i = 0; i < (IC - 100); i++) {
        /* Using %03X to show the 12-bit word in 3 hex digits */
        /* code_image[i].type usually holds 'A', 'R', or 'E' [cite: 190] */
        printf("Addr %04d: %03X (ARE: %c)\n", 
                i + 100, 
                code_image[i].word, 
                code_image[i].type ? code_image[i].type : 'A'); 
    }

    printf("\n--- DEBUG: Data Image (Memory) ---\n");
    /* DC represents the number of words in the data segment  */
    for (i = 0; i < DC; i++) {
        printf("Addr %04d: %03X\n", i, data_image[i].word);
    }
}