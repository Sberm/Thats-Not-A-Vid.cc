#include "decode.h"
#include "config.h"

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

char create_byte(int x, int y, unsigned char* buffer) {
    char c = 0;
    int col = FRAME_SIZE.width / 2;

    for (int i = 0; i < 8; i++) {

        // std::cout << "should be 0 or 255:" << (unsigned int)buffer[x * 2 * FRAME_SIZE.width + y * 2] << std::endl;

        if (buffer[x * 2 * FRAME_SIZE.width + y * 2] > (unsigned char)128) {
            c |= (1 << (7 - i) );
        }
        int index = x * col + y;
        index++;
        x = index / col;
        y = index % col;
    }

    return c;
}

void get_header(cv::Mat& frame, std::vector<char>& buffer, int col, int row, std::string& file_name, std::string& file_length) {

	// get file name and file size
	if (frame.empty()) {
		std::cout << "Video source is empty" << std::endl;
		exit(0);
	}

    std::vector<cv::Mat> spl;

	cv::split(frame, spl);

	enum write_mode {
		FileName,
		FileLength,
		Normal
	};

	write_mode wm = FileName;

	for (int i = 0; i < col * row; i += 8) {
		char c = create_byte(i / col, i % col, spl[0].data);
		if (wm == Normal) {
			buffer.push_back(c);
		} else if (wm == FileName) {
			if (c == '\n') {
				wm = FileLength;
			} else {
				file_name.append(1, c);
			}
		} else if (wm == FileLength) {
			if (c == '\n') {
				wm = Normal;
			} else {
				file_length.append(1, c);
			}
		}
	}
	
}

void decode(std::string path) {
    // read video
    cv::VideoCapture input_video(path);

    cv::Mat frame;
    cv::Mat trash;
    std::vector<cv::Mat> spl;

    int col = FRAME_SIZE.width / 2;
    int row = FRAME_SIZE.height / 2;

    int count_write = 0;

	// create buffer to store the data in a frame besides filename
	std::vector<char> buffer(0);

	// read the first two frames
	input_video >> trash;
	input_video >> frame;

    // create output file
	std::string decoded_path, file_name, file_length_str;
	get_header(frame, buffer, col, row, file_name, file_length_str);


	decoded_path = "../decoded/" + file_name;

	const int file_length = std::stoi(file_length_str);

    std::ofstream file(decoded_path, std::ofstream::binary);
    if (!file.is_open()) {
        printf("Path %s doesn't exit\n", decoded_path.c_str());
        return;
    }

	// handle the rest of the data
	for (char c : buffer) {
		file.write(&c, sizeof c);
		count_write++;
	}

    // read each frame
    for (;;) {

        // two frames
        input_video >> trash;
        input_video >> frame;

        if (frame.empty()) {
            break;
        }
        cv::split(frame, spl);

        for (int i = 0; i < col * row; i += 8) {
            char c = create_byte(i / col, i % col, spl[0].data);
            file.write(&c, sizeof c);
			count_write++;

			if (count_write >= file_length) {
				break;
			}
        }
    }

    std::cout << "Decoded file size: " << count_write / 1000 << " KB" << std::endl;

    file.close();
    input_video.release();

    std::cout << "Decoding complete" << std::endl;
}
