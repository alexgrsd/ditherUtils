// first create an ascii image from input, we'll se the rest later :D
#include "textRain.hpp"
#include "../masking/mask.hpp"


cv::Mat imageToAscii(const cv::Mat& inputImage, int  block_size) 
// convert image to ascii art representation with block_size the size of each character block in pixels
{ // assuming image are only black and white and we know pixel sizes (= block_size in here, so each character represents block_size x block_size pixels)

    (void)block_size; // unused for now

    cv::Mat asciiImage;
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    int** mask = createMask(inputImage); (void)mask; // create a mask from the image
    char ascii_char = '#'; (void)ascii_char; // using only one character for now

    asciiImage = cv::Mat::zeros(rows, cols, CV_8UC1); // we start from a black image

    return asciiImage;

}