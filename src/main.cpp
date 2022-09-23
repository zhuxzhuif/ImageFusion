/*
 * File:          main.cpp
 * Author:        Baozhe Zhang
 * Created Date:  09/22/2022
 * Usage:         Main file of Image Fusion
 * LICENCE:       MIT
 */

#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <random>
namespace fs = std::filesystem;

#include <gflags/gflags.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <xml_writer.hpp>

DEFINE_string(target_dir, "", "Directory of the target images");
DEFINE_string(background_dir, "", "Directory of the background images");
DEFINE_string(output_dir, "output", "Directory of the output results");

// =0= | =1= | =2=
// ---------------
// =3= | =4= | =5=
// ---------------
// =6= | =7= | =8=
const int LOCATION_NUM = 9;
const int LOCATIONS[LOCATION_NUM][4] =
    {
        // x_left_up, y_left_up, x_right_down, y_right_down
        {0, 0, 213, 159},       // 0
        {213, 0, 426, 159},     // 1
        {426, 0, 639, 159},     // 2
        {0, 159, 213, 319},     // 3
        {213, 159, 426, 319},   // 4
        {426, 159, 639, 319},   // 5
        {0, 319, 213, 479},     // 6
        {213, 319, 426, 479},   // 7
        {426, 319, 639, 479}    // 8
    };
const int RANDOM_SPAWN_NUM = 4;
const int ROTATION_NUM = 8;
const int SCALE_NUM = 4;
const int BLUR_NUM = 2;
const int LIGHT_NUM = 4;

// For a target image with a background image
// the program will generate this number of images
// LOCATION_NUM * RANDOM_SPAWN_NUM * ROTATION_NUM * SCALE_NUM * BLUR_NUM * LIGHT_NUM

// Generate random number
int gen_random_number(int lower, int upper)
{
  static std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> distr(lower, upper);
  return distr(generator);
}

// Extract the target from a large image with the white background
void crop_image(const cv::Mat &src, cv::Mat &des)
{
  cv::Mat target_image_gray;
  cv::cvtColor(src, target_image_gray, cv::COLOR_BGR2GRAY);

  cv::Mat target_image_gray_threshold;
  cv::threshold(target_image_gray, target_image_gray_threshold, 240, 255, cv::THRESH_BINARY_INV);

  cv::imshow("window", target_image_gray_threshold);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
  cv::Mat gray_threshold_morphed;
  cv::morphologyEx(target_image_gray_threshold,
                   gray_threshold_morphed, cv::MORPH_CLOSE, kernel);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(gray_threshold_morphed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  std::sort(contours.begin(), contours.end(),
            [=](const auto &a, const auto &b)
            {
              return cv::contourArea(a) > cv::contourArea(b);
            });
  auto contour = contours[0];
  cv::Rect rect = cv::boundingRect(contour);
  des = src(cv::Range(rect.y, rect.y+rect.height),
                            cv::Range(rect.x, rect.x+rect.width));
}

// Helper function
// Fuse a target image into a background image
void Fuse(const std::string &target_dir,
     const std::string &background_dir,
     const std::string &filename,
     const std::string &background_filename)
{
  // TODO: target image will be constructed and destructed at each loop
  auto target_image = cv::imread(target_dir + filename + ".png");
  std::cout << target_dir + filename + ".png" << std::endl;
  std::cout << "Target image " << filename << " size: " << target_image.size() << std::endl;

  // ========== crop ==============
  cv::Mat target_cropped;
  crop_image(target_image, target_cropped);
  cv::imshow("small", target_cropped);
  // ========== crop ==============

  auto background = cv::imread(background_dir + background_filename + ".jpg");
  cv::resize(background, background, cv::Size(640, 480));
  cv::imshow("background", background);
  // messy loops
  for (auto location_idx = 0; location_idx < LOCATION_NUM; location_idx++) {
  for (auto random_idx = 0; random_idx < RANDOM_SPAWN_NUM; random_idx++) {
  for (auto rotation_idx = 0; rotation_idx < ROTATION_NUM; rotation_idx++) {
  for (auto scale_idx = 0; scale_idx < SCALE_NUM; scale_idx++) {
  for (auto blur_idx = 0; blur_idx < BLUR_NUM; blur_idx++) {
  for (auto light_idx = 0; light_idx < LIGHT_NUM; light_idx++) {
    // do the work
    std::cout << "Location idx: " << location_idx << std::endl;
    auto target_size = target_cropped.size();

    // Rotation ref: https://stackoverflow.com/questions/22041699/rotate-an-image-without-cropping-in-opencv-in-c
    int angle = 360 / ROTATION_NUM * rotation_idx;
    cv::Point2i center((target_cropped.cols-1)/2.0, (target_cropped.rows-1)/2.0);


    int x_left = LOCATIONS[location_idx][0];
    int y_up = LOCATIONS[location_idx][1];
    int x_right = LOCATIONS[location_idx][2] - target_size.width;
    int y_down = LOCATIONS[location_idx][3] - target_size.height;
    int x = gen_random_number(x_left, x_right);
    int y = gen_random_number(y_up, y_down);
    auto background_copy = background.clone();
    cv::Mat roi(background_copy, cv::Rect(x, y, target_size.width, target_size.height));
    target_cropped.copyTo(roi);
    cv::imshow("copied background", background_copy);
    cv::waitKey(10);

  }}}}}}



}

int main(int argc, char **argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::string target_dir = FLAGS_target_dir;
  std::string background_dir = FLAGS_background_dir;
  std::string output_dir = FLAGS_output_dir;
  if (target_dir.back() != '/') target_dir.push_back('/');
  if (background_dir.back() != '/') background_dir.push_back('/');
  if (output_dir.back() != '/') output_dir.push_back('/');



  xml_writer::XMLLabelFile file;

  auto start_time = std::chrono::high_resolution_clock::now();
  for (const auto &target_entry : fs::directory_iterator(target_dir)) {

    auto image_start_time = std::chrono::high_resolution_clock::now();
    auto filename = target_entry.path().filename().replace_extension("").string();

    for (const auto &background_entry : fs::directory_iterator(background_dir)) {
      // TODO: Do the OpenCV transform work here
      auto background_filename = background_entry.path().filename().replace_extension("").string();
      Fuse(target_dir, background_dir, filename, background_filename);
    }

    auto image_end_time = std::chrono::high_resolution_clock::now();
    auto image_time = std::chrono::duration_cast<std::chrono::milliseconds>(image_end_time - image_start_time);
    std::cout << "\033[1;31mTarget image " << filename << " transform used "
              << image_time.count() << " ms\033[0m" << std::endl;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
  std::cout << "Time used: " << time.count() << " seconds" << std::endl;

  return 0;
}
