# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.0.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.0.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/furukawa/research/structured_indoor_modeling/code/calibration

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/furukawa/research/structured_indoor_modeling/code/calibration

# Include any dependencies generated for this target.
include CMakeFiles/align_images_cli.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/align_images_cli.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/align_images_cli.dir/flags.make

CMakeFiles/align_images_cli.dir/align_images_cli.cc.o: CMakeFiles/align_images_cli.dir/flags.make
CMakeFiles/align_images_cli.dir/align_images_cli.cc.o: align_images_cli.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/furukawa/research/structured_indoor_modeling/code/calibration/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/align_images_cli.dir/align_images_cli.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/align_images_cli.dir/align_images_cli.cc.o -c /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images_cli.cc

CMakeFiles/align_images_cli.dir/align_images_cli.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/align_images_cli.dir/align_images_cli.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images_cli.cc > CMakeFiles/align_images_cli.dir/align_images_cli.cc.i

CMakeFiles/align_images_cli.dir/align_images_cli.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/align_images_cli.dir/align_images_cli.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images_cli.cc -o CMakeFiles/align_images_cli.dir/align_images_cli.cc.s

CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.requires:
.PHONY : CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.requires

CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.provides: CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.requires
	$(MAKE) -f CMakeFiles/align_images_cli.dir/build.make CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.provides.build
.PHONY : CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.provides

CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.provides.build: CMakeFiles/align_images_cli.dir/align_images_cli.cc.o

CMakeFiles/align_images_cli.dir/align_images.cc.o: CMakeFiles/align_images_cli.dir/flags.make
CMakeFiles/align_images_cli.dir/align_images.cc.o: align_images.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/furukawa/research/structured_indoor_modeling/code/calibration/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/align_images_cli.dir/align_images.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/align_images_cli.dir/align_images.cc.o -c /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images.cc

CMakeFiles/align_images_cli.dir/align_images.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/align_images_cli.dir/align_images.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images.cc > CMakeFiles/align_images_cli.dir/align_images.cc.i

CMakeFiles/align_images_cli.dir/align_images.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/align_images_cli.dir/align_images.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/furukawa/research/structured_indoor_modeling/code/calibration/align_images.cc -o CMakeFiles/align_images_cli.dir/align_images.cc.s

CMakeFiles/align_images_cli.dir/align_images.cc.o.requires:
.PHONY : CMakeFiles/align_images_cli.dir/align_images.cc.o.requires

CMakeFiles/align_images_cli.dir/align_images.cc.o.provides: CMakeFiles/align_images_cli.dir/align_images.cc.o.requires
	$(MAKE) -f CMakeFiles/align_images_cli.dir/build.make CMakeFiles/align_images_cli.dir/align_images.cc.o.provides.build
.PHONY : CMakeFiles/align_images_cli.dir/align_images.cc.o.provides

CMakeFiles/align_images_cli.dir/align_images.cc.o.provides.build: CMakeFiles/align_images_cli.dir/align_images.cc.o

CMakeFiles/align_images_cli.dir/transformation.cc.o: CMakeFiles/align_images_cli.dir/flags.make
CMakeFiles/align_images_cli.dir/transformation.cc.o: transformation.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/furukawa/research/structured_indoor_modeling/code/calibration/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/align_images_cli.dir/transformation.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/align_images_cli.dir/transformation.cc.o -c /Users/furukawa/research/structured_indoor_modeling/code/calibration/transformation.cc

CMakeFiles/align_images_cli.dir/transformation.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/align_images_cli.dir/transformation.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/furukawa/research/structured_indoor_modeling/code/calibration/transformation.cc > CMakeFiles/align_images_cli.dir/transformation.cc.i

CMakeFiles/align_images_cli.dir/transformation.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/align_images_cli.dir/transformation.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/furukawa/research/structured_indoor_modeling/code/calibration/transformation.cc -o CMakeFiles/align_images_cli.dir/transformation.cc.s

CMakeFiles/align_images_cli.dir/transformation.cc.o.requires:
.PHONY : CMakeFiles/align_images_cli.dir/transformation.cc.o.requires

CMakeFiles/align_images_cli.dir/transformation.cc.o.provides: CMakeFiles/align_images_cli.dir/transformation.cc.o.requires
	$(MAKE) -f CMakeFiles/align_images_cli.dir/build.make CMakeFiles/align_images_cli.dir/transformation.cc.o.provides.build
.PHONY : CMakeFiles/align_images_cli.dir/transformation.cc.o.provides

CMakeFiles/align_images_cli.dir/transformation.cc.o.provides.build: CMakeFiles/align_images_cli.dir/transformation.cc.o

# Object files for target align_images_cli
align_images_cli_OBJECTS = \
"CMakeFiles/align_images_cli.dir/align_images_cli.cc.o" \
"CMakeFiles/align_images_cli.dir/align_images.cc.o" \
"CMakeFiles/align_images_cli.dir/transformation.cc.o"

# External object files for target align_images_cli
align_images_cli_EXTERNAL_OBJECTS =

align_images_cli: CMakeFiles/align_images_cli.dir/align_images_cli.cc.o
align_images_cli: CMakeFiles/align_images_cli.dir/align_images.cc.o
align_images_cli: CMakeFiles/align_images_cli.dir/transformation.cc.o
align_images_cli: CMakeFiles/align_images_cli.dir/build.make
align_images_cli: /usr/local/lib/libopencv_videostab.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_video.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_ts.a
align_images_cli: /usr/local/lib/libopencv_superres.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_stitching.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_photo.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_ocl.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_objdetect.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_nonfree.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_ml.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_legacy.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_imgproc.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_highgui.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_gpu.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_flann.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_features2d.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_core.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_contrib.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_calib3d.2.4.9.dylib
align_images_cli: /usr/local/lib/libceres.1.9.0.dylib
align_images_cli: /usr/local/lib/libopencv_nonfree.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_ocl.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_gpu.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_photo.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_objdetect.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_legacy.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_video.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_ml.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_calib3d.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_features2d.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_highgui.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_imgproc.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_flann.2.4.9.dylib
align_images_cli: /usr/local/lib/libopencv_core.2.4.9.dylib
align_images_cli: /usr/local/lib/libglog.dylib
align_images_cli: CMakeFiles/align_images_cli.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable align_images_cli"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/align_images_cli.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/align_images_cli.dir/build: align_images_cli
.PHONY : CMakeFiles/align_images_cli.dir/build

CMakeFiles/align_images_cli.dir/requires: CMakeFiles/align_images_cli.dir/align_images_cli.cc.o.requires
CMakeFiles/align_images_cli.dir/requires: CMakeFiles/align_images_cli.dir/align_images.cc.o.requires
CMakeFiles/align_images_cli.dir/requires: CMakeFiles/align_images_cli.dir/transformation.cc.o.requires
.PHONY : CMakeFiles/align_images_cli.dir/requires

CMakeFiles/align_images_cli.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/align_images_cli.dir/cmake_clean.cmake
.PHONY : CMakeFiles/align_images_cli.dir/clean

CMakeFiles/align_images_cli.dir/depend:
	cd /Users/furukawa/research/structured_indoor_modeling/code/calibration && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/furukawa/research/structured_indoor_modeling/code/calibration /Users/furukawa/research/structured_indoor_modeling/code/calibration /Users/furukawa/research/structured_indoor_modeling/code/calibration /Users/furukawa/research/structured_indoor_modeling/code/calibration /Users/furukawa/research/structured_indoor_modeling/code/calibration/CMakeFiles/align_images_cli.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/align_images_cli.dir/depend

