#ifndef CLI_H
#define CLI_H
#include <string>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "encode.h"
#include "decode.h"

std::string get_input();
void process_input();
void clear_line(int line_num);
void start_cli();
void clear_input_buffer();

#endif
