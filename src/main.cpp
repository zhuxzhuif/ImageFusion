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

// TODO: doc is created and deleted each time this function is called
//       may be use a struct and only change the value in it
void generate_xml(const std::string &output_dir,
                  const std::string &store_name,
                  const BoundingBox &bounding_box)
{
  pugi::xml_document doc;
  auto annotation = doc.append_child("annotation");
  annotation.append_attribute("verified") = "yes";

  // folder
  auto folder = annotation.append_child("folder");
  folder.append_child(pugi::node_pcdata).set_value("None");
  // filename
  auto filename = annotation.append_child("filename");
  filename.append_child(pugi::node_pcdata).set_value(store_name.c_str());
  // path
  auto path = annotation.append_child("path");
  path.append_child(pugi::node_pcdata).set_value("None");
  // source
  auto source = annotation.append_child("source");
  auto database = source.append_child("database");
  database.append_child(pugi::node_pcdata).set_value("Unknown");
  // size
  auto size = annotation.append_child("size");
  auto width = size.append_child("width");
  width.append_child(pugi::node_pcdata).set_value("640");
  auto height = size.append_child("height");
  height.append_child(pugi::node_pcdata).set_value("480");
  auto depth = size.append_child("depth");
  depth.append_child(pugi::node_pcdata).set_value("3");
  // segmented
  auto segmented = annotation.append_child("segmented");
  segmented.append_child(pugi::node_pcdata).set_value("0");
  // object
  auto object = annotation.append_child("object");

  auto object_name = object.append_child("name");
  auto temp_object_name = bounding_box.target_name.c_str();
  object_name.append_child(pugi::node_pcdata).set_value(temp_object_name);

  auto object_pose = object.append_child("pose");
  object_pose.append_child(pugi::node_pcdata).set_value("Unspecified");

  auto object_truncated = object.append_child("truncated");
  object_truncated.append_child(pugi::node_pcdata).set_value("0");

  auto object_difficult = object.append_child("difficult");
  object_difficult.append_child(pugi::node_pcdata).set_value("0");

  auto object_bndbox = object.append_child("bndbox");
  auto bndbox_xmin = object_bndbox.append_child("xmin");
  bndbox_xmin.append_child(pugi::node_pcdata).set_value(
      std::to_string(bounding_box.x_min).c_str());
  auto bndbox_ymin = object_bndbox.append_child("ymin");
  bndbox_ymin.append_child(pugi::node_pcdata).set_value(
      std::to_string(bounding_box.y_min).c_str());
  auto bndbox_xmax = object_bndbox.append_child("xmax");
  bndbox_xmax.append_child(pugi::node_pcdata).set_value(
      std::to_string(bounding_box.x_max).c_str());
  auto bndbox_ymax = object_bndbox.append_child("ymax");
  bndbox_ymax.append_child(pugi::node_pcdata).set_value(
      std::to_string(bounding_box.y_max).c_str());

  doc.save_file((store_name + ".xml").c_str());


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

  BoundingBox box;
  box.x_max = 0;
  box.x_min = 0;
  box.y_max = 0;
  box.y_min = 0;
  box.target_name = "target";
  generate_xml("hello", "hello", box);

  return 0;
}
