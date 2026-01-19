#include <iostream>
#include <opencv2/opencv.hpp>
#include "engine/effects/textRain.hpp"

int main() {
    cv::Mat img_test = cv::imread("../img/angel.png", cv::IMREAD_COLOR);
    if (img_test.empty()) {
        std::cerr << "Error: cannot read ../img/angel.png\n";
        return -1;
    }
    int block_size = 8; // size of each character block in pixels
    int font_size = 10; // font size for characters
    int threshold = 80; // brightness threshold
    double upscale_factor = 2.0; // upscale factor for better quality

    // create charset of printable ascii characters
    std::string charset;
    for (char c = 33; c <= 126; ++c) { // printable ascii characters
        charset += c;
    }

    // create ascii video from image
    int fps = 24;
    int length_seconds = 5;
    std::string output_folder = "../img/";
    int ret = imageToVideoAsciiRandomChar(img_test, block_size, font_size, threshold, upscale_factor, charset, fps, length_seconds, output_folder);
    if (ret != 0) {
        std::cerr << "Error creating ascii video\n";
        return ret;
    }

    std::cout << "Ascii video written to ../img/seq/ascii_video.mp4\n";
    return 0;
}
