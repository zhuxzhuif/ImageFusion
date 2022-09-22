/*
 * File:          main.cpp
 * Author:        Baozhe Zhang
 * Created Date:  09/22/2022
 * Usage:         Main file of Image Fusion
 * LICENCE:       MIT
 */

#include <iostream>
#include <string>

#include <gflags/gflags.h>
#include <opencv2/opencv.hpp>

#include <pugixml.hpp>

struct BoundingBox
{
  int x_min;
  int x_max;
  int y_min;
  int y_max;
  std::string target_name;
};

void generate_xml(const std::string &output_dir,
                  const BoundingBox &bounding_box)
{

}

DEFINE_string(target_dir, "", "Directory of the target images");
DEFINE_string(background_dir, "", "Directory of the background images");
DEFINE_string(output_dir, "output", "Directory of the output results");

int main(int argc, char **argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  const std::string target_dir = FLAGS_target_dir;
  const std::string background_dir = FLAGS_background_dir;
  const std::string output_dir = FLAGS_output_dir;
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
