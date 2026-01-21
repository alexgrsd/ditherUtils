#include <opencv2/opencv.hpp>

cv::Mat rgbShiftGlitch(const cv::Mat& inputImage, int red_amount, int green_amount, int blue_amount)
{
    CV_Assert(!inputImage.empty());

    cv::Mat bgr;
    if (inputImage.channels() == 3) {
        bgr = inputImage;
    } else if (inputImage.channels() == 1) {
        cv::cvtColor(inputImage, bgr, cv::COLOR_GRAY2BGR);
    } else if (inputImage.channels() == 4) {
        cv::cvtColor(inputImage, bgr, cv::COLOR_BGRA2BGR);
    } else {
        CV_Assert(false && "Unsupported number of channels");
    }

    cv::Mat outputImage = bgr.clone();
    std::vector<cv::Mat> channels;
    cv::split(outputImage, channels);

    // Shift Red channel (channels[2])
    cv::Mat red_shifted;
    cv::Mat translation_red = (cv::Mat_<double>(2,3) << 1, 0, red_amount, 0, 1, 0);
    cv::warpAffine(channels[2], red_shifted, translation_red, channels[2].size(),
                   cv::INTER_LINEAR, cv::BORDER_WRAP);
    channels[2] = red_shifted;

    // Shift Green channel (channels[1])
    cv::Mat green_shifted;
    cv::Mat translation_green = (cv::Mat_<double>(2,3) << 1, 0, green_amount, 0, 1, 0);
    cv::warpAffine(channels[1], green_shifted, translation_green, channels[1].size(),
                   cv::INTER_LINEAR, cv::BORDER_WRAP);
    channels[1] = green_shifted;

    // Shift Blue channel (channels[0])
    cv::Mat blue_shifted;
    cv::Mat translation_blue = (cv::Mat_<double>(2,3) << 1, 0, blue_amount, 0, 1, 0);
    cv::warpAffine(channels[0], blue_shifted, translation_blue, channels[0].size(),
                   cv::INTER_LINEAR, cv::BORDER_WRAP);
    channels[0] = blue_shifted;

    cv::merge(channels, outputImage);
    return outputImage;
}
