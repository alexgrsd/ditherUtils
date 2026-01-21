// textRain.hpp
#pragma once

#include <opencv2/opencv.hpp>
#include <string>
int **createMaskLowRes(const cv::Mat &inputImage, int block_size, int threshold);

// API principale
cv::Mat imageToAscii(const cv::Mat &inputImage,
                     int block_size,
                     int font_size,
                     int threshold,
                     double upscale_factor,
                     std::string charset);






cv::Mat imageToVideoAsciiText(const cv::Mat &inputImage,
                              int block_size,
                              int font_size,
                              int threshold,
                              double upscale_factor,
                              std::string text);
