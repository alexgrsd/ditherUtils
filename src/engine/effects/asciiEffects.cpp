// first create an ascii image from input, we'll se the rest later :D
#include "asciiEffects.hpp"

#include <cstdlib> // std::rand
#include <cmath>   // std::round

// free memory allocated for the mask
static void freeMask(int **mask, int mask_rows)
{
    if (!mask)
        return;
    for (int i = 0; i < mask_rows; ++i)
        delete[] mask[i];
    delete[] mask;
}

// write a character on a cell of the image at position x,y with given font size
static void writeTextOnCell(cv::Mat &image, char ascii_char,
                            int x, int y,
                            int cell_size,
                            int font_size)
{
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = font_size / 30.0;
    int thickness = (font_size <= 10) ? 1 : 2;

    int baseline = 0;
    cv::Size text_size = cv::getTextSize(std::string(1, ascii_char),
                                         font_face, font_scale, thickness, &baseline);

    int text_x = x + (cell_size - text_size.width) / 2;
    int text_y = y + (cell_size + text_size.height) / 2;

    cv::putText(image, std::string(1, ascii_char),
                cv::Point(text_x, text_y),
                font_face, font_scale,
                cv::Scalar(255),
                thickness,
                cv::LINE_AA);
}

// main API function to convert image to ascii
cv::Mat imageToAscii(const cv::Mat &inputImage,
                     int block_size,
                     int font_size,
                     int threshold,
                     double upscale_factor,
                     std::string charset)
{
    CV_Assert(block_size > 0);
    CV_Assert(!inputImage.empty());

    static bool seeded = false;
    if (!seeded) {
        seeded = true;
        std::srand((unsigned)cv::getTickCount());
    }

    cv::Mat processedImage;
    if (upscale_factor > 1.0)
        cv::resize(inputImage, processedImage, cv::Size(), upscale_factor, upscale_factor, cv::INTER_CUBIC);
    else
        processedImage = inputImage;

    int rows = processedImage.rows;
    int cols = processedImage.cols;

    int mRows = rows / block_size;
    int mCols = cols / block_size;

    if (charset.empty())
        charset = "#";

    int **mask = createMaskLowRes(processedImage, block_size, threshold);
    cv::Mat asciiImage = cv::Mat::zeros(rows, cols, CV_8UC1);

    if (!mask) {
        return asciiImage;
    }

    char ascii_char = charset[0];

    for (int i = 0; i + block_size <= rows; i += block_size)
    {
        int mask_i = i / block_size;
        if (mask_i < 0 || mask_i >= mRows || !mask[mask_i])
            continue;

        for (int j = 0; j + block_size <= cols; j += block_size)
        {
            int mask_j = j / block_size;
            if (mask_j < 0 || mask_j >= mCols)
                continue;

            if (mask[mask_i][mask_j] == 0)
            {
                continue;
            }

            // écrire le char centré dans la cellule (block_size)
            writeTextOnCell(asciiImage, ascii_char, j, i, block_size, font_size);

            ascii_char = charset[std::rand() % charset.size()];
        }
    }

    freeMask(mask, mRows);
    return asciiImage;
}

cv::Mat imageToVideoAsciiText(const cv::Mat &inputImage,
                              int block_size,
                              int font_size,
                              int threshold,
                              double upscale_factor,
                              std::string text)
{
    // takes an image and make a ascii with characters from the given text instead of sequential ones, create a glitch effect i guess
    return imageToAscii(inputImage, block_size, font_size, threshold, upscale_factor, text); // just use text as charset for now
}










// ------------------ deprecated but kept for reference ------------------
/*int imageToVideoAsciiRandomChar(const cv::Mat &inputImage,
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

    cv::Size size(
        int(std::round(inputImage.cols * upscale_factor)),
        int(std::round(inputImage.rows * upscale_factor)));

    // creates frames in temp_folder
    for (int i = 0; i < total_frames; ++i)
    {
        cv::Mat asciiImage = imageToAscii(inputImage, block_size, font_size, threshold, upscale_factor, charset);

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
}*/