#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include "./effects/asciiEffects.hpp"
#include "./effects/glitch.hpp"

int imageSequenceToVideo(std::string input_folder,
                         int total_frames,
                         int fps,
                         std::string output_folder);

// generate image sequence from input image + effects parameters
int generateImageSequence(const cv::Mat &inputImage,
                                         int block_size,
                                         int font_size,
                                         int threshold,
                                         double upscale_factor,
                                         std::string charset,
                                         int total_frames,
                                         std::string output_folder);