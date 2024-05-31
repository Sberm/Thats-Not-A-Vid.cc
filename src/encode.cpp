#include "encode.h"
#include "config.h"

#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <stdio.h>

inline bool get_bit(char c, int bit)
{
	return (c >> bit) & 1;
}

void set_pixel(int i, int j, unsigned char* buffer_frame, bool bit) {
	// 0 -> black, 1 -> white
	int row = FRAME_SIZE.width;
	int up_left = (i * row + j) * 2 * 3;

	unsigned bit_to_set = bit ? 255 : 0;

	buffer_frame[up_left + 0] = bit_to_set;
	buffer_frame[up_left + 1] = bit_to_set;
	buffer_frame[up_left + 2] = bit_to_set;

	buffer_frame[up_left + 3 + 0] = bit_to_set;
	buffer_frame[up_left + 3 + 1] = bit_to_set;
	buffer_frame[up_left + 3 + 2] = bit_to_set;

	buffer_frame[up_left + row * 3 + 0] = bit_to_set;
	buffer_frame[up_left + row * 3 + 1] = bit_to_set;
	buffer_frame[up_left + row * 3 + 2] = bit_to_set;

	buffer_frame[up_left + (row + 1)* 3 + 0] = bit_to_set;
	buffer_frame[up_left + (row + 1)* 3 + 1] = bit_to_set;
	buffer_frame[up_left + (row + 1)* 3 + 2] = bit_to_set;
}

void writeFrame(cv::VideoWriter &outputVideo, char *buffer, int length) {
	int mat_size = 640 * 360;
	unsigned char* buffer_frame = new unsigned char[mat_size * 2 * 2 * 3];
	int index = 0;

	std::cout << "\x1b?25l" << std::endl;

	for (int i = 0; i < length; i++) {
		// print encode progress
		std::cout
		<< "\x1b[1A"
		<< "\x1b[K"
		<< (float)i / length * 100 << "%" << std::endl;

		char c = buffer[i];
		for (int j = 0; j < 8; j++) {
			int px = index / 640;
			int py = index % 640;
			set_pixel(px, py, buffer_frame, get_bit(c, 7 - j));
			index++;
		}

		// if a frame is fully written, write frame to outputVideo
		if (index >= mat_size) {
			// index of pixels
			index = 0;
			cv::Mat frame = cv::Mat(FRAME_SIZE, CV_8UC3, buffer_frame);
			// write 2 frames
			outputVideo.write(frame);
			outputVideo.write(frame);
			// set buffer_frame to 0
			std::memset(buffer_frame, 0, mat_size * 4 * 3 * sizeof(unsigned char));
		}
	}

	// fill up the blank and write the remaining pixels
	if (index > 0) {
		index = 0;
		cv::Mat frame = cv::Mat(FRAME_SIZE, CV_8UC3, buffer_frame);
		// write 2 frames
		outputVideo.write(frame);
		outputVideo.write(frame);
	}

	delete[] buffer_frame;
}

std::string get_file_name(std::string path) {
	size_t start;
	start = path.find_last_of("/");
	return path.substr(start + 1, std::string::npos - start);
}

void encode(std::string path) {
	// read file
	std::ifstream file(path, std::ifstream::binary);
	if (!file.is_open()) {
		printf("Can't open file: %s\n",path.c_str());
		return;
	}

	// get length of file:
	file.seekg (0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);

	int content_length = length;

	// get file name, place on the first line(append a \n at the end)
	std::string file_name = get_file_name(path) + '\n';

	// write file length into buffer
	std::string file_length = std::to_string(length) + '\n';

	length = length + file_name.length() + file_length.length();

	// allocate memory based on length:
	char *buffer = new char[length];
	printf("Reading data from %s\n", path.c_str());

	// write file name and data to buffer
	strcpy((char *)buffer, file_name.c_str());
	strcpy((char *)buffer + sizeof(char) * file_name.length(), file_length.c_str());
	file.read((char *)buffer + sizeof(char) * file_name.length() + sizeof(char) * file_length.length(), content_length);

	// encode
	cv::Mat src;
	cv::VideoWriter outputVideo;
	std::string fileName = "../output/output.mp4";
	int frameRate = 30;

	outputVideo.open(fileName, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), frameRate, FRAME_SIZE, true);
	if (!outputVideo.isOpened()) {
		std::cout  << "Could not open the output video for write: " << fileName << std::endl;
		return;
	}

	writeFrame(outputVideo, buffer, length);

	// print complete
	std::cout
	<< "\x1b[1A"
	<< "\x1b[K"
	<< "Encoding complete" << std::endl;

	// delete buffer
	delete[] buffer; 
}
