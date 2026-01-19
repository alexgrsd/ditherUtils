#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat imageToAscii(const cv::Mat& inputImage, int  block_size, int font_size, int threshold, double upscale_factor); // convert image to ascii art representation with block_size the size of each character block in pixels
void writeTextOnCell(cv::Mat& image, char ascii_char, int x, int y, int font_size); // write a character on a cell of the image at position x,y with given font size
cv::Mat imageToVideoAscii(const cv::Mat& inputImage, int block_size, int font_size, int threshold, double upscale_factor); // make an image and make a video ascii version (not implemented yet)