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
const int RANDOM_SPAWN_NUM = 4;
const int ROTATION_NUM = 8;
const int SCALE_NUM = 4;
const int BLUR_NUM = 2;
const int LIGHT_NUM = 4;

// For a target image with a background image
// the program will generate this number of images
// LOCATION_NUM * RANDOM_SPAWN_NUM * ROTATION_NUM * SCALE_NUM * BLUR_NUM * LIGHT_NUM

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
  cv::Mat target_image_gray;

  // messy loops
  for (auto location_idx = 0; location_idx < LOCATION_NUM; location_idx++) {
  for (auto random_idx = 0; random_idx < RANDOM_SPAWN_NUM; random_idx++) {
  for (auto rotation_idx = 0; rotation_idx < ROTATION_NUM; rotation_idx++) {
  for (auto scale_idx = 0; scale_idx < SCALE_NUM; scale_idx++) {
  for (auto blur_idx = 0; blur_idx < BLUR_NUM; blur_idx++) {
  for (auto light_idx = 0; light_idx < LIGHT_NUM; light_idx++) {
    // do the work

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
