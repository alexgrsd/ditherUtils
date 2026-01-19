// first create an ascii image from input, we'll se the rest later :D
#include "textRain.hpp"
#include "../masking/mask.hpp"

void writeTextOnCell(cv::Mat& image, char ascii_char, int x, int y, int font_size) {
    // Define font face and scale
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = font_size / 30.0; // Adjust scale based on desired font size
    int thickness = (font_size <= 10) ? 1 : 2;

    // Get text size
    int baseline = 0;
    cv::Size text_size = cv::getTextSize(std::string(1, ascii_char), font_face, font_scale, thickness, &baseline);

    // Calculate position to center the text in the cell
    int text_x = x + (font_size - text_size.width) / 2;
    int text_y = y + (font_size + text_size.height) / 2; // y is the baseline for putText

    // Put the text on the image (anti-aliased)
    cv::putText(image, std::string(1, ascii_char),
                cv::Point(text_x, text_y),
                font_face, font_scale,
                cv::Scalar(255),
                thickness,
                cv::LINE_AA);
}


cv::Mat imageToAscii(const cv::Mat& inputImage, int block_size, int font_size, int threshold, double upscale_factor)
// Convert input image to an ASCII-like image.
// - block_size: size (in pixels) of one character cell (block_size x block_size)
// - font_size:  desired font size for cv::putText (your writeTextOnCell decides how to interpret it)
// - threshold:  used to build a low-resolution mask from the image
// Assumption: inputImage is black/white (or at least thresholdable).
// we can upscale image before processing to get better results

{
    CV_Assert(block_size > 0);
    CV_Assert(!inputImage.empty());

    cv::Mat asciiImage;
    cv::Mat processedImage;
    // Upscale the image if needed
    if (upscale_factor > 1.0) {
        cv::resize(inputImage, processedImage, cv::Size(), upscale_factor, upscale_factor, cv::INTER_CUBIC);
    } else {
        processedImage = inputImage;
    }

    int rows = processedImage.rows;
    int cols = processedImage.cols;

    // Create a low-resolution mask based on block size and threshold
    int** mask = createMaskLowRes(processedImage, block_size, threshold);

    int mRows = rows / block_size;
    int mCols = cols / block_size;

    char ascii_char = 'A'; // starting character

    // Output image: start with a black canvas 
    asciiImage = cv::Mat::zeros(rows, cols, CV_8UC1);

    // Iterate over the image by blocks
    for (int i = 0; i + block_size <= rows; i += block_size) {
        for (int j = 0; j + block_size <= cols; j += block_size) {

            // Convert pixel coordinates to cell coordinates
            int mask_i = i / block_size;
            int mask_j = j / block_size;

            // avoid reading mask out of bounds (and avoid null rows)
            if (!mask) continue;
            if (mask_i < 0 || mask_i >= mRows || mask_j < 0 || mask_j >= mCols) continue;
            if (!mask[mask_i]) continue;

            if (mask[mask_i][mask_j] == 0) {
                // leave block empty (black)
                // (optional: asciiImage is already black)
                for (int bi = 0; bi < block_size; ++bi) {
                    for (int bj = 0; bj < block_size; ++bj) {
                        asciiImage.at<uchar>(i + bi, j + bj) = 0;
                    }
                }
            } else {
                // write ascii character in the block
                writeTextOnCell(asciiImage, ascii_char, j, i, font_size);
                ascii_char++;
                if (ascii_char > 'z') {
                    ascii_char = 'A'; // wrap around after 'z'
                }
            }
        }
    }

    return asciiImage;
}

