# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/pkavasse/dev/dji-stream

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/pkavasse/dev/dji-stream/build

# Include any dependencies generated for this target.
include CMakeFiles/test_liveview.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_liveview.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_liveview.dir/flags.make

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o: CMakeFiles/test_liveview.dir/flags.make
CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o: ../examples/liveview/test_liveview_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/pkavasse/dev/dji-stream/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o -c /mnt/c/Users/pkavasse/dev/dji-stream/examples/liveview/test_liveview_main.cc

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/pkavasse/dev/dji-stream/examples/liveview/test_liveview_main.cc > CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.i

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/pkavasse/dev/dji-stream/examples/liveview/test_liveview_main.cc -o CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.s

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.requires:

.PHONY : CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.requires

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.provides: CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.requires
	$(MAKE) -f CMakeFiles/test_liveview.dir/build.make CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.provides.build
.PHONY : CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.provides

CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.provides.build: CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o


# Object files for target test_liveview
test_liveview_OBJECTS = \
"CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o"

# External object files for target test_liveview
test_liveview_EXTERNAL_OBJECTS =

bin/test_liveview: CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o
bin/test_liveview: CMakeFiles/test_liveview.dir/build.make
bin/test_liveview: libedge_sample.a
bin/test_liveview: CMakeFiles/test_liveview.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/pkavasse/dev/dji-stream/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/test_liveview"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_liveview.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_liveview.dir/build: bin/test_liveview

.PHONY : CMakeFiles/test_liveview.dir/build

CMakeFiles/test_liveview.dir/requires: CMakeFiles/test_liveview.dir/examples/liveview/test_liveview_main.cc.o.requires

.PHONY : CMakeFiles/test_liveview.dir/requires

CMakeFiles/test_liveview.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_liveview.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_liveview.dir/clean

CMakeFiles/test_liveview.dir/depend:
	cd /mnt/c/Users/pkavasse/dev/dji-stream/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/pkavasse/dev/dji-stream /mnt/c/Users/pkavasse/dev/dji-stream /mnt/c/Users/pkavasse/dev/dji-stream/build /mnt/c/Users/pkavasse/dev/dji-stream/build /mnt/c/Users/pkavasse/dev/dji-stream/build/CMakeFiles/test_liveview.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_liveview.dir/depend

