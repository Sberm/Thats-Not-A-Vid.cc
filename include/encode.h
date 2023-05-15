#ifndef ENCODE_H
#define ENCODE_H

#include <string>
#include <opencv2/opencv.hpp>

inline bool get_bit(char c, int bit);
void writeFrame(cv::VideoWriter &outputVideo, char *buffer, int length);
void encode (std::string path);

#endif