#include <stdio.h>
#include <string>
#include "cli.h"
#include "encode.h"
#include "decode.h"

std::string get_input() {
    char buffer[256];
    scanf("%s", buffer);
    return std::string(buffer);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void process_input() {
    while (true) {
        printf("Please enter command (encode/decode/exit):\n");
        std::string input = get_input();
        if (input == "encode") {
            printf("Please enter the path of the file:\n");
            char buf[256];
            scanf("%s", buf);
            encode(std::string(buf));
        } else if (input == "decode") {
            printf("Please enter the path of the video:\n");
            char buf[256] = {0};

            clear_input_buffer();
            fgets(buf, (sizeof(buf) - 1) * sizeof(char), stdin);

            if (buf[0] == '\n') {
                decode();
            } else {
                decode(std::string(buf));
            }

        } else if (input == "exit") {
            exit(0);
        } else {
            printf("Command not found\n");
        }
    }
}

void start_cli() {
    process_input();
}
