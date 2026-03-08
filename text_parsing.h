#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

int tokenize(char *line, char *args[]);

int is_empty_line(char *line);

int is_comment(char *line);

#endif