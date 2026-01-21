#include "imageEngine.hpp"

int imageSequenceToVideo(std::string input_folder,
                         int total_frames,
                         int fps,
                         std::string output_folder)
{
    if (!input_folder.empty() && input_folder.back() != '/')
        input_folder += '/';
    if (!output_folder.empty() && output_folder.back() != '/')
        output_folder += '/';

    cv::Mat first = cv::imread(input_folder + "ascii_frame_0.png", cv::IMREAD_GRAYSCALE);
    if (first.empty())
    {
        std::cerr << "Error: Missing frame 0: " << (input_folder + "ascii_frame_0.png") << "\n";
        return -1;
    }

    cv::Size size = first.size();

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
        cv::Mat g = cv::imread(input_folder + "ascii_frame_" + std::to_string(i) + ".png", cv::IMREAD_GRAYSCALE);
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

// generate a sequence of image with added effects of choice (will be more effects than just ascii in the future)
int generateImageSequence(const cv::Mat &inputImage,
                                         int block_size, int font_size, int threshold,
                                         double upscale_factor, std::string charset,
                                         int total_frames,
                                         std::string output_folder)
{
    CV_Assert(!inputImage.empty());

    std::string temp_folder = "../img/tmp/";
    if (!temp_folder.empty() && temp_folder.back() != '/')
        temp_folder += '/';

    if (!output_folder.empty() && output_folder.back() != '/')
        output_folder += '/';

    cv::Size size(
        int(std::round(inputImage.cols * upscale_factor)),
        int(std::round(inputImage.rows * upscale_factor)));

    for (int i = 0; i < total_frames; ++i)
    {
        cv::Mat asciiImage = imageToAscii(inputImage, block_size, font_size, threshold, upscale_factor, charset);

        if (asciiImage.size() != size)
            cv::resize(asciiImage, asciiImage, size, 0, 0, cv::INTER_NEAREST);

        cv::imwrite(temp_folder + "ascii_frame_" + std::to_string(i) + ".png", asciiImage);
        std::cout << "Written frame " << i << std::endl;
    }

    return 0;
}