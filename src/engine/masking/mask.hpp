#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

int** createMask(const cv::Mat& image); // Function to apply a mask to an image, takes a cv::Mat as input and returns the mask as a 2D array 
int** createMaskLowRes(const cv::Mat& image, int cell_size, int threshold); // create a low res mask based on cell size
cv::Mat applyMask(int** mask,int res_x,int res_y); // create grey image from mask for debug purposes