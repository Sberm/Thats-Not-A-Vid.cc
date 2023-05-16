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

void decode(std::string path) {
    // read video
    cv::VideoCapture input_video(path);

    // create new file
    std::string decoded_path = "../decoded/output_file.png";
    std::ofstream file(decoded_path, std::ofstream::binary);
    if (!file.is_open()) {
        printf("Path %s doesn't exit\n", decoded_path.c_str());
        return;
    }

    cv::Mat frame;
    cv::Mat trash;
    std::vector<cv::Mat> spl;

    int col = FRAME_SIZE.width / 2;
    int row = FRAME_SIZE.height / 2;

    int count_write = 0;

    // read each frame
    for (;;) {

        // two frames
        input_video >> trash;
        input_video >> frame;

        if (frame.empty()) {
            break;
        }
        cv::split(frame, spl);

        // std::cout << spl[0].rows << "x" << spl[0].cols << std::endl;

        for (int i = 0; i < col * row; i += 8) {

            char c = create_byte(i / col, i % col, spl[0].data);

            file.write(&c, sizeof c);
            count_write++;
        }
    }

    std::cout << "Decoded file size: " << count_write / 1000 << std::endl;

    file.close();
    input_video.release();

    std::cout << "Decoding complete" << std::endl;
}