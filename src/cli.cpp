#include "cli.h"

std::string get_input() {
	char buffer[256];
	scanf("%s", buffer);
	return std::string(buffer);
}

void clear_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

void clear_line(int line_num) {
	while (line_num--) {
		std::cout << "\033[1A" << "\033[2K";
	}
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
			sleep(2);
			clear_line(6);
		} else if (input == "decode") {
			printf("Please enter the path of the video:\n");
			char buf[256] = {0};

			clear_input_buffer();
			fgets(buf, (sizeof(buf) - 1) * sizeof(char), stdin);

			if (buf[0] == '\n') {
				decode();
			} else {
				// remove the last \n that was read by fgets
				for (int i = sizeof(buf) - 1; i >= 0; i--) {
					if (buf[i] == '\n') {
						buf[i] = '\0';
						break;
					}
				}
				decode(std::string(buf));
			}
			clear_line(6);
			sleep(2);
		} else if (input == "exit") {
			exit(0);
		} else {
			printf("Command not found\n");
			sleep(1);
			clear_line(3);
		}
	}
}

void start_cli() {
	process_input();
}
