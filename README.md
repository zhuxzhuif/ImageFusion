# Image Fusion

Image Fusion, a utility small C++ program generating different 
pictures of objects with different backgrounds.

## Dependencies

### OpenCV

Install the OpenCV firs.

### gflags

This library is used for parsing the command-line arguments. 
To install it, in your terminal type
```bash
sudo apt-get install libflags-dev
```

### pugixml

This library is used for writing xml files. 
It is already included in the `dependencies` folder.



## Compile
This project uses `CMake` to build the source code. 
To build this project, in your terminal (assume in the project folder)
```bash
mkdir build
cd build 
cmake ..
make -j4
```

## Run

The name of the executable is `ImageFusion`. 
Suppose you have a resource folder where the target and background images are stored.
To run the program, in your terminal 
```bash
cd build
./ImageFusion -target_dir=<your_target_directory> \
 -background_dir=<your_background_directory> \ 
 -output_dir=<the_output_directory>
```