#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* The filename is defined once here for consistency */
#define OUTPUT_FILENAME "output.txt"

/* Internal helper: Handles the bit-to-string conversion */
static void write_bits_to_file(FILE *fp, short value) {
    int i;
    for (i = 11; i >= 0; i--) {
        if ((value >> i) & 1)
            fprintf(fp, "1");
        else
            fprintf(fp, "0");
        
        if (i % 4 == 0 && i != 0) fprintf(fp, " ");
    }
}

/* * CALL THIS: When you have the symbol head.
 * It creates (or clears) the file and writes the table.
 */
void save_symbol_table(symbol *head) {
    symbol *temp = head;
    /* "w" mode: starts a fresh file or overwrites an old one */
    FILE *fp = fopen(OUTPUT_FILENAME, "w"); 
    
    if (!fp) {
        perror("Failed to open file for symbol table");
        return;
    }

    fprintf(fp, "--- DEBUG: Symbol Table ---\n");
    fprintf(fp, "%-20s | %-8s | %-15s\n", "Label Name", "Address", "Attribute");
    fprintf(fp, "----------------------------------------------------------\n");

    while (temp != NULL) {
        fprintf(fp, "%-20s | %-8d | ", temp->name, temp->address);
        
        switch(temp->atr) {
            case code:     fprintf(fp, "code");     break;
            case data:     fprintf(fp, "data");     break;
            case external: fprintf(fp, "external"); break;
            case entry:    fprintf(fp, "entry");    break;
            default:       fprintf(fp, "unknown");  break;
        }
        
        fprintf(fp, "\n");
        temp = temp->next;
    }

    fprintf(fp, "\n");
    fclose(fp);
}

/* * CALL THIS: When you finish the machine image.
 * It appends the images to the end of the existing file.
 */
void save_machine_images() {
    int i;
    int instructions_count = IC - 100;
    int total_words = instructions_count + DC;
    
    FILE *fp = fopen(OUTPUT_FILENAME, "a"); 

    if (!fp) {
        perror("Failed to open file for machine images");
        return;
    }

    fprintf(fp, "--- DEBUG: Combined Memory Image ---\n");
    
    for (i = 0; i < total_words; i++) {
        short w;
        char c;
        
        /* Addr starts at 100 and increments for every word */
        fprintf(fp, "Addr %04d: ", i + 100);

        if (i < instructions_count) {
            /* We are still in the Code segment */
            w = code_image[i].word;
            c = code_image[i].type ? code_image[i].type : 'A';
        } else {
            /* We have moved into the Data segment */
            /* We subtract instructions_count to reset the index to 0 for data_image */
            int data_idx = i - instructions_count;
            w = data_image[data_idx].word;
            c = data_image[data_idx].type ? data_image[data_idx].type : 'A';
        }

        write_bits_to_file(fp, w);
        fprintf(fp, " (ARE: %c)\n", c);
    }

    fprintf(fp, "\n--- End of Image ---\n---------------------------\n");
    fclose(fp);
}