// first create an ascii image from input, we'll se the rest later :D
#include "textRain.hpp"
#include "../masking/mask.hpp"

#include <cstdlib>  // std::rand
#include <cmath>    // std::round

// helper to avoid memory leaks (mask is allocated with new[] in createMaskLowRes)
static void freeMask(int** mask, int mask_rows) {
    if (!mask) return;
    for (int i = 0; i < mask_rows; ++i) delete[] mask[i];
    delete[] mask;
}

void writeTextOnCell(cv::Mat &image, char ascii_char, int x, int y, int font_size)
{
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

cv::Mat imageToAscii(const cv::Mat &inputImage, 
                        int block_size, 
                        int font_size, 
                        int threshold, 
                        double upscale_factor, 
                        std::string charset)
// Convert input image to an ASCII-like image.
// - block_size: size (in pixels) of one character cell (block_size x block_size)
// - font_size:  desired font size for cv::putText (your writeTextOnCell decides how to interpret it)
// - threshold:  used to build a low-resolution mask from the image
// upscale_factor: factor to upscale the input image before processing (to improve quality)
// char used are random printable ascii characters from 33 to 126
{
    CV_Assert(block_size > 0);
    CV_Assert(!inputImage.empty());

    cv::Mat asciiImage;
    cv::Mat processedImage;
    // Upscale the image if needed
    if (upscale_factor > 1.0)
    {
        cv::resize(inputImage, processedImage, cv::Size(), upscale_factor, upscale_factor, cv::INTER_CUBIC);
    }
    else
    {
        processedImage = inputImage;
    }

    int rows = processedImage.rows;
    int cols = processedImage.cols;

    // Create a low-resolution mask based on block size and threshold
    int **mask = createMaskLowRes(processedImage, block_size, threshold);

    int mRows = rows / block_size;
    int mCols = cols / block_size;

    if (charset.empty()) charset = "#";
    char ascii_char = charset[0]; // starting character
    
    // Output image: start with a black canvas
    asciiImage = cv::Mat::zeros(rows, cols, CV_8UC1);

    // Iterate over the image by blocks
    for (int i = 0; i + block_size <= rows; i += block_size)
    {
        for (int j = 0; j + block_size <= cols; j += block_size)
        {

            // Convert pixel coordinates to cell coordinates
            int mask_i = i / block_size;
            int mask_j = j / block_size;

            // avoid reading mask out of bounds (and avoid null rows)
            if (!mask)
                continue;
            if (mask_i < 0 || mask_i >= mRows || mask_j < 0 || mask_j >= mCols)
                continue;
            if (!mask[mask_i])
                continue;

            if (mask[mask_i][mask_j] == 0)
            {
                // leave block empty (black)
                // (optional: asciiImage is already black)
                for (int bi = 0; bi < block_size; ++bi)
                {
                    for (int bj = 0; bj < block_size; ++bj)
                    {
                        asciiImage.at<uchar>(i + bi, j + bj) = 0;
                    }
                }
            }
            else
            {
                // write ascii character in the block
                writeTextOnCell(asciiImage, ascii_char, j, i, font_size);
                ascii_char = charset[std::rand() % charset.size()]; // next character from charset
            }
        }
    }

    // free mask to avoid memory leaks
    freeMask(mask, mRows);

    return asciiImage;
}

int imageToVideoAsciiRandomChar(const cv::Mat &inputImage,
                                int block_size, int font_size, int threshold,
                                double upscale_factor, std::string charset,
                                int fps, int length_seconds,
                                std::string output_folder)
{
    // takes an image and make a ascii with random characters from the given charset
    std::string temp_folder = "../img/tmp/";

    // ensure folders end with '/'
    if (!temp_folder.empty() && temp_folder.back() != '/')
        temp_folder += '/';
    if (!output_folder.empty() && output_folder.back() != '/')
        output_folder += '/';

    int total_frames = length_seconds * fps;

    // FIX: define a stable target size (prevents cropping/size mismatch)
    cv::Size size(
        int(std::round(inputImage.cols * upscale_factor)),
        int(std::round(inputImage.rows * upscale_factor))
    );

    // creates frames in temp_folder
    for (int i = 0; i < total_frames; ++i)
    {
        cv::Mat asciiImage = imageToAscii(inputImage, block_size, font_size, threshold, upscale_factor, charset);

        // FIX: force consistent frame size before saving
        if (asciiImage.size() != size)
        {
            cv::resize(asciiImage, asciiImage, size, 0, 0, cv::INTER_NEAREST);
        }

        cv::imwrite(temp_folder + "ascii_frame_" + std::to_string(i) + ".png", asciiImage);
        std::cout << "Written frame " << i << std::endl;
    }

    // then create video from frames in output_folder
    cv::VideoWriter video_writer;
    bool ok = video_writer.open(
        output_folder + "ascii_video.mp4",
        cv::CAP_FFMPEG,
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        size,
        true);

    if (!ok || !video_writer.isOpened())
    {
        std::cerr << "Error: Could not open the video writer at: "
                  << (output_folder + "ascii_video.mp4") << "\n";
        return -1;
    }

    for (int i = 0; i < total_frames; i++)
    {
        cv::Mat g = cv::imread(temp_folder + "ascii_frame_" + std::to_string(i) + ".png", cv::IMREAD_GRAYSCALE);
        if (g.empty())
        {
            std::cerr << "Missing frame " << i << "\n";
            break;
        }

        if (g.size() != size)
            cv::resize(g, g, size, 0, 0, cv::INTER_NEAREST);

        cv::Mat bgr;
        cv::cvtColor(g, bgr, cv::COLOR_GRAY2BGR);
        video_writer.write(bgr);
    }

    video_writer.release();
    std::cout << "Ascii video written to " << output_folder << "ascii_video.mp4\n";
    return 0;
}

cv::Mat imageToVideoAsciiText(const cv::Mat &inputImage, 
                                int block_size, 
                                int font_size, 
                                int threshold, 
                                double upscale_factor, 
                                std::string text)
{
    // takes an image and make a ascii with characters from the given text instead of sequential ones, create a glitch effect i guess
    (void)text;                                                                              // currently unused
    return imageToAscii(inputImage, block_size, font_size, threshold, upscale_factor, text); // just use text as charset for now
}
