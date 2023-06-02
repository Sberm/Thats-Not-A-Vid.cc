#ifndef DECODE_H
#define DECODE_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

void decode(std::string path = "../output/output.mp4");
void get_header(cv::Mat& frame, std::vector<char>& buffer, int col, int row, std::string& file_name, std::string& file_length);
char create_byte(int x, int y, unsigned char* buffer);

#endif
