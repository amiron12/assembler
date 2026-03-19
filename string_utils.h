#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include "structs.h"

void get_next_word(char **line, char **word);

int tokenize(char *line, char *args[], file_data *fs);

int is_empty_line(char *line);

int is_comment(char *line);

void extention(file_data *fs, char *ext);

void clean_string(char **str, char c);

#endif