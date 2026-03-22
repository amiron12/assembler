#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include "structs.h"

/* Extracts the first word from a line and updates the line pointer */
void get_next_word(char **line, char **word);

/* Splits a line into comma-separated arguments and validates commas */
int tokenize(char *line, char *args[], file_data *fs);

/* Checks if a line contains only whitespace characters */
int is_empty_line(char *line);

/* Determines if a line starts with a semicolon, indicating a comment */
int is_comment(char *line);

/* Appends an extension to the file name inside the file_data structure */
void extention(file_data *fs, char *ext);

/* Strips specific characters (like ':', '#', '%', '"') from a string */
void clean_string(char **str, char c);

#endif
