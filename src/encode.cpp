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

    if (!bit) {
        buffer_frame[up_left + 0] = (unsigned char)0;
        buffer_frame[up_left + 1] = (unsigned char)0;
        buffer_frame[up_left + 2] = (unsigned char)0;

        buffer_frame[up_left + 3 + 0] = (unsigned char)0;
        buffer_frame[up_left + 3 + 1] = (unsigned char)0;
        buffer_frame[up_left + 3 + 2] = (unsigned char)0;

        buffer_frame[up_left + row * 3 + 0] = (unsigned char)0;
        buffer_frame[up_left + row * 3 + 1] = (unsigned char)0;
        buffer_frame[up_left + row * 3 + 2] = (unsigned char)0;

        buffer_frame[up_left + (row + 1)* 3 + 0] = (unsigned char)0;
        buffer_frame[up_left + (row + 1)* 3 + 1] = (unsigned char)0;
        buffer_frame[up_left + (row + 1)* 3 + 2] = (unsigned char)0;
    } else {
        buffer_frame[up_left + 0] = (unsigned char)255;
        buffer_frame[up_left + 1] = (unsigned char)255;
        buffer_frame[up_left + 2] = (unsigned char)255;

        buffer_frame[up_left + 3 + 0] = (unsigned char)255;
        buffer_frame[up_left + 3 + 1] = (unsigned char)255;
        buffer_frame[up_left + 3 + 2] = (unsigned char)255;

        buffer_frame[up_left + row * 3 + 0] = (unsigned char)255;
        buffer_frame[up_left + row * 3 + 1] = (unsigned char)255;
        buffer_frame[up_left + row * 3 + 2] = (unsigned char)255;

        buffer_frame[up_left + (row + 1)* 3 + 0] = (unsigned char)255;
        buffer_frame[up_left + (row + 1)* 3 + 1] = (unsigned char)255;
        buffer_frame[up_left + (row + 1)* 3 + 2] = (unsigned char)255;
    }
}

void writeFrame(cv::VideoWriter &outputVideo, char *buffer, int length) {

    // TODO: finish writing each frame after the buffer is full(640 * 360 * 3)

    int mat_size = 640 * 360;
    unsigned char* buffer_frame = new unsigned char[mat_size * 2 * 2 * 3];
    int index = 0;

    std::cout << "\x1b?25l" << std::endl;

    for (int i = 0; i < length; i++) {

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
        if (index >= mat_size) {
            index = 0;
            cv::Mat frame = cv::Mat(FRAME_SIZE, CV_8UC3, buffer_frame);
            // write 2 frames
            outputVideo.write(frame);
            outputVideo.write(frame);

            // set buffer_frame to 0
            std::memset(buffer_frame, 0, mat_size * 4 * 3 * sizeof(unsigned char));
        }
    }

    if (index >= 1) {
        index = 0;
        cv::Mat frame = cv::Mat(FRAME_SIZE, CV_8UC3, buffer_frame);
        // write 2 frames
        outputVideo.write(frame);
        outputVideo.write(frame);
    }

    delete[] buffer_frame;
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

    char *buffer = new char[length];
    printf("Reading data from %s\n", path.c_str());
    file.read(buffer, length);

    // encode
    cv::Mat src;
    cv::VideoWriter outputVideo;
    std::string fileName = "../output/output.mp4";
    int frameRate = 30;

    outputVideo.open(fileName, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), frameRate, FRAME_SIZE, true);
    if (!outputVideo.isOpened())
    {
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