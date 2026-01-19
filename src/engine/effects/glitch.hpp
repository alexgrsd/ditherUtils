#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat rgbShiftGlitch(const cv::Mat& inputImage, int red_amount, int green_amount, int blue_amount); // apply rgb shift glitch effect to input image with given shift amount in pixels
cv::Mat pixelSortGlitch(const cv::Mat& inputImage, int threshold); // apply pixel sorting glitch effect to input image with given brightness threshold (not sure about implementing it though)
cv::Mat ScanlineGlitch(const cv::Mat& inputImage, int line_height, int shift_amount); // apply scanline glitch effect to input image with given line height and shift amount