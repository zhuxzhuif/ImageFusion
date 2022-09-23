//
// Created by baozhe on 22-9-23.
//

#ifndef IMAGEFUSION_INCLUDE_XML_WRITER_HPP_
#define IMAGEFUSION_INCLUDE_XML_WRITER_HPP_

#include <iostream>
#include <pugixml.hpp>

namespace xml_writer
{

struct BoundingBox
{
  int x_min;
  int x_max;
  int y_min;
  int y_max;
  std::string target_name;
};

struct XMLLabelFile
{
  using node = pugi::xml_node;
  pugi::xml_document doc;
  node annotation;
  node folder;
  node filename;
  node path;
  node source;
  node database;
  node size;
  node width;
  node height;
  node depth;
  node segmented;
  node object;
  node object_name;
  node object_pose;
  node object_truncated;
  node object_difficult;
  node object_bndbox;
  node bndbox_xmin;
  node bndbox_ymin;
  node bndbox_xmax;
  node bndbox_ymax;

  node filename_value;
  node path_value;
  node object_name_value;
  node xmin_value;
  node ymin_value;
  node xmax_value;
  node ymax_value;

  // Constructor
  XMLLabelFile()
  {
    annotation = doc.append_child("annotation");
    annotation.append_attribute("verified") = "yes";

    // folder
    folder = annotation.append_child("folder");
    folder.append_child(pugi::node_pcdata).set_value("None");
    // filename
    filename = annotation.append_child("filename");
    filename_value = filename.append_child(pugi::node_pcdata); // can be changed
    filename_value.set_value("");
    // path
    path = annotation.append_child("path");
    path_value = path.append_child(pugi::node_pcdata); // can be changed
    path_value.set_value("");
    // source
    source = annotation.append_child("source");
    database = source.append_child("database");
    database.append_child(pugi::node_pcdata).set_value("Unknown");
    // size
    size = annotation.append_child("size");
    width = size.append_child("width");
    width.append_child(pugi::node_pcdata).set_value("640");
    height = size.append_child("height");
    height.append_child(pugi::node_pcdata).set_value("480");
    depth = size.append_child("depth");
    depth.append_child(pugi::node_pcdata).set_value("3");
    // segmented
    segmented = annotation.append_child("segmented");
    segmented.append_child(pugi::node_pcdata).set_value("0");
    // object
    object = annotation.append_child("object");

    object_name = object.append_child("name");
    object_name_value = object_name.append_child(pugi::node_pcdata); // can be changed
    object_name_value.set_value("");

    object_pose = object.append_child("pose");
    object_pose.append_child(pugi::node_pcdata).set_value("Unspecified");

    object_truncated = object.append_child("truncated");
    object_truncated.append_child(pugi::node_pcdata).set_value("0");

    object_difficult = object.append_child("difficult");
    object_difficult.append_child(pugi::node_pcdata).set_value("0");

    object_bndbox = object.append_child("bndbox");
    bndbox_xmin = object_bndbox.append_child("xmin");
    xmin_value = bndbox_xmin.append_child(pugi::node_pcdata); // can be changed
    xmin_value.set_value("");
    bndbox_ymin = object_bndbox.append_child("ymin");
    ymin_value = bndbox_ymin.append_child(pugi::node_pcdata); // can be changed
    ymin_value.set_value("");
    bndbox_xmax = object_bndbox.append_child("xmax");
    xmax_value = bndbox_xmax.append_child(pugi::node_pcdata); // can be changed
    xmax_value.set_value("");
    bndbox_ymax = object_bndbox.append_child("ymax");
    ymax_value = bndbox_ymax.append_child(pugi::node_pcdata); // can be changed
    ymax_value.set_value("");
  }

  // filename contains no extension
  void ChangeValue(const std::string &output_dir,
                   const std::string &filename,
                   const std::string &object_name,
                   int x_min, int y_min,
                   int x_max, int y_max)
  {
    filename_value.set_value((filename + ".jpg").c_str());
    path_value.set_value((output_dir + filename + ".jpg").c_str());
    object_name_value.set_value(object_name.c_str());
    xmin_value.set_value(std::to_string(x_min).c_str());
    ymin_value.set_value(std::to_string(y_min).c_str());
    xmax_value.set_value(std::to_string(x_max).c_str());
    ymax_value.set_value(std::to_string(y_max).c_str());
  }
  pugi::xml_document &GetDocument()
  {
    return doc;
  }

  // User can call this function to save the file
  bool SaveFile(const std::string &output_dir,
                const std::string &filename,
                const std::string &object_name,
                int x_min, int y_min,
                int x_max, int y_max)
  {
    ChangeValue(output_dir, filename, object_name,
                x_min, y_min, x_max, y_max);
    if (GetDocument().save_file((output_dir + filename + ".xml").c_str()))
    {
      // Nothing happens
      return true;
    } else {
      std::cerr << "[" + output_dir + filename + ".xml" +"]" << " cannot be saved ..." << std::endl;
      return false;
    }
  }
};


}

#endif //IMAGEFUSION_INCLUDE_XML_WRITER_HPP_
