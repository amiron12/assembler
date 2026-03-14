#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H
#include "structs.h"

int tokenize(char *line, char *args[], file_data *fs);

int is_empty_line(char *line);

int is_comment(char *line);

#endif