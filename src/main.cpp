#include <iostream>
#include "mask.hpp"

int main ()
{

    cv::Mat img_test = cv::imread("../img/statue_floyd_x4.jpg", cv::IMREAD_UNCHANGED);
    if (img_test.empty()) {
        std::cerr << "Error: Could not load image." << std::endl;
        return -1;
    }
    std::cout << "Image loaded with size: " << img_test.cols << "x" << img_test.rows << std::endl;
    int** mask = createMask(img_test);
    if (!mask) {
        std::cerr << "Error: Could not create mask." << std::endl;
        return -1;
    }
    std::cout << "Mask created." << std::endl;
    cv::Mat render = applyMask(mask, img_test.rows, img_test.cols);
    if (render.empty()) {
        std::cerr << "Error: Could not apply mask." << std::endl;
        return -1;
    }
    std::cout << "Mask applied to debug image." << std::endl;
    cv::imwrite("../img/masked_debug.png", render);
    if (render.empty()) {
        std::cerr << "Error: Could not save debug image." << std::endl;
        return -1;
    }
    std::cout << "Debug image saved as masked_debug.png" << std::endl;
}
