# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/user/Edge-SDK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Edge-SDK/build

# Include any dependencies generated for this target.
include CMakeFiles/edge_sample.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/edge_sample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/edge_sample.dir/flags.make

CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o: ../examples/common/image_processor.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o -c /home/user/Edge-SDK/examples/common/image_processor.cc

CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/common/image_processor.cc > CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.i

CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/common/image_processor.cc -o CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.s

CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o: ../examples/common/image_processor_yolovfastest.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o -c /home/user/Edge-SDK/examples/common/image_processor_yolovfastest.cc

CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/common/image_processor_yolovfastest.cc > CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.i

CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/common/image_processor_yolovfastest.cc -o CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.s

CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o: ../examples/common/util_misc.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o -c /home/user/Edge-SDK/examples/common/util_misc.cc

CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/common/util_misc.cc > CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.i

CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/common/util_misc.cc -o CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.s

CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o: ../examples/init/key_store_default.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o -c /home/user/Edge-SDK/examples/init/key_store_default.cc

CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/init/key_store_default.cc > CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.i

CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/init/key_store_default.cc -o CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.s

CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o: ../examples/init/pre_init.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o -c /home/user/Edge-SDK/examples/init/pre_init.cc

CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/init/pre_init.cc > CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.i

CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/init/pre_init.cc -o CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.s

CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o: ../examples/liveview/ffmpeg_stream_decoder.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o -c /home/user/Edge-SDK/examples/liveview/ffmpeg_stream_decoder.cc

CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/liveview/ffmpeg_stream_decoder.cc > CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.i

CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/liveview/ffmpeg_stream_decoder.cc -o CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.s

CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o: ../examples/liveview/image_processor_thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o -c /home/user/Edge-SDK/examples/liveview/image_processor_thread.cc

CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/liveview/image_processor_thread.cc > CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.i

CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/liveview/image_processor_thread.cc -o CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.s

CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o: ../examples/liveview/sample_liveview.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o -c /home/user/Edge-SDK/examples/liveview/sample_liveview.cc

CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/liveview/sample_liveview.cc > CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.i

CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/liveview/sample_liveview.cc -o CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.s

CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o: ../examples/liveview/stream_decoder.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o -c /home/user/Edge-SDK/examples/liveview/stream_decoder.cc

CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/liveview/stream_decoder.cc > CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.i

CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/liveview/stream_decoder.cc -o CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.s

CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o: CMakeFiles/edge_sample.dir/flags.make
CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o: ../examples/liveview/stream_processor_thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o -c /home/user/Edge-SDK/examples/liveview/stream_processor_thread.cc

CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Edge-SDK/examples/liveview/stream_processor_thread.cc > CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.i

CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Edge-SDK/examples/liveview/stream_processor_thread.cc -o CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.s

# Object files for target edge_sample
edge_sample_OBJECTS = \
"CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o" \
"CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o" \
"CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o" \
"CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o" \
"CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o" \
"CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o" \
"CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o" \
"CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o" \
"CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o" \
"CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o"

# External object files for target edge_sample
edge_sample_EXTERNAL_OBJECTS =

libedge_sample.a: CMakeFiles/edge_sample.dir/examples/common/image_processor.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/common/image_processor_yolovfastest.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/common/util_misc.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/init/key_store_default.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/init/pre_init.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/liveview/ffmpeg_stream_decoder.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/liveview/image_processor_thread.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/liveview/sample_liveview.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/liveview/stream_decoder.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/examples/liveview/stream_processor_thread.cc.o
libedge_sample.a: CMakeFiles/edge_sample.dir/build.make
libedge_sample.a: CMakeFiles/edge_sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Edge-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX static library libedge_sample.a"
	$(CMAKE_COMMAND) -P CMakeFiles/edge_sample.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/edge_sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/edge_sample.dir/build: libedge_sample.a

.PHONY : CMakeFiles/edge_sample.dir/build

CMakeFiles/edge_sample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/edge_sample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/edge_sample.dir/clean

CMakeFiles/edge_sample.dir/depend:
	cd /home/user/Edge-SDK/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Edge-SDK /home/user/Edge-SDK /home/user/Edge-SDK/build /home/user/Edge-SDK/build /home/user/Edge-SDK/build/CMakeFiles/edge_sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/edge_sample.dir/depend

