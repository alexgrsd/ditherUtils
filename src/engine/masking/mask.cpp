#include "mask.hpp"

// create a simple mask based on pixel intensity (only works for single channel images but will do)
int** createMask(const cv::Mat& image) {
    int** mask = new int*[image.rows];
    for (int i = 0; i < image.rows; ++i) {
        mask[i] = new int[image.cols];
        for (int j = 0; j < image.cols; ++j) {
            mask[i][j] = (image.at<uchar>(i, j) > 128) ? 1 : 0;
        }
    }
    return mask;
}

int** createMaskLowRes(const cv::Mat& image, int cell_size, int threshold) { // create a low res mask based on cell size
    // resize to low resolution
    int low_res_rows = image.rows / cell_size;
    int low_res_cols = image.cols / cell_size;
    cv::Mat low_res_image;
    cv::resize(image, low_res_image, cv::Size(low_res_cols, low_res_rows), 0, 0, cv::INTER_AREA);
    // create mask based on low res image
    int** mask = new int*[low_res_rows];
    for (int i = 0; i < low_res_rows; ++i) {
        mask[i] = new int[low_res_cols];
        for (int j = 0; j < low_res_cols; ++j) {
            mask[i][j] = (low_res_image.at<uchar>(i, j) > threshold) ? 1 : 0;
        }
    }
    return mask;
}

// create a greyscale image for debug purposes based on the mask
cv::Mat applyMask(int** mask, int res_x, int res_y) {
    cv::Mat maskedImage(res_x, res_y, CV_8UC1, cv::Scalar(0));
    for (int i = 0; i < res_x; ++i) {
        for (int j = 0; j < res_y; ++j) {
            if (mask[i][j] == 0) {
                maskedImage.at<uchar>(i, j) = 0; // Set pixel to black if mask is 0
            }
            else {
                maskedImage.at<uchar>(i, j) = 128; // Set pixel to gray if mask is 1 (for debug visibility)
            }
        }
    }
    return maskedImage;
}