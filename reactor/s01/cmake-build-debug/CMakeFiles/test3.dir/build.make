# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/zy/programfiles/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/zy/programfiles/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zy/Projects/snippet/reactor/s01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zy/Projects/snippet/reactor/s01/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test3.dir/flags.make

CMakeFiles/test3.dir/Eventloop.cpp.o: CMakeFiles/test3.dir/flags.make
CMakeFiles/test3.dir/Eventloop.cpp.o: ../Eventloop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test3.dir/Eventloop.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test3.dir/Eventloop.cpp.o -c /home/zy/Projects/snippet/reactor/s01/Eventloop.cpp

CMakeFiles/test3.dir/Eventloop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test3.dir/Eventloop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zy/Projects/snippet/reactor/s01/Eventloop.cpp > CMakeFiles/test3.dir/Eventloop.cpp.i

CMakeFiles/test3.dir/Eventloop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test3.dir/Eventloop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zy/Projects/snippet/reactor/s01/Eventloop.cpp -o CMakeFiles/test3.dir/Eventloop.cpp.s

CMakeFiles/test3.dir/Poller.cpp.o: CMakeFiles/test3.dir/flags.make
CMakeFiles/test3.dir/Poller.cpp.o: ../Poller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test3.dir/Poller.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test3.dir/Poller.cpp.o -c /home/zy/Projects/snippet/reactor/s01/Poller.cpp

CMakeFiles/test3.dir/Poller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test3.dir/Poller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zy/Projects/snippet/reactor/s01/Poller.cpp > CMakeFiles/test3.dir/Poller.cpp.i

CMakeFiles/test3.dir/Poller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test3.dir/Poller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zy/Projects/snippet/reactor/s01/Poller.cpp -o CMakeFiles/test3.dir/Poller.cpp.s

CMakeFiles/test3.dir/Channel.cpp.o: CMakeFiles/test3.dir/flags.make
CMakeFiles/test3.dir/Channel.cpp.o: ../Channel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test3.dir/Channel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test3.dir/Channel.cpp.o -c /home/zy/Projects/snippet/reactor/s01/Channel.cpp

CMakeFiles/test3.dir/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test3.dir/Channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zy/Projects/snippet/reactor/s01/Channel.cpp > CMakeFiles/test3.dir/Channel.cpp.i

CMakeFiles/test3.dir/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test3.dir/Channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zy/Projects/snippet/reactor/s01/Channel.cpp -o CMakeFiles/test3.dir/Channel.cpp.s

CMakeFiles/test3.dir/test3.cpp.o: CMakeFiles/test3.dir/flags.make
CMakeFiles/test3.dir/test3.cpp.o: ../test3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test3.dir/test3.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test3.dir/test3.cpp.o -c /home/zy/Projects/snippet/reactor/s01/test3.cpp

CMakeFiles/test3.dir/test3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test3.dir/test3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zy/Projects/snippet/reactor/s01/test3.cpp > CMakeFiles/test3.dir/test3.cpp.i

CMakeFiles/test3.dir/test3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test3.dir/test3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zy/Projects/snippet/reactor/s01/test3.cpp -o CMakeFiles/test3.dir/test3.cpp.s

# Object files for target test3
test3_OBJECTS = \
"CMakeFiles/test3.dir/Eventloop.cpp.o" \
"CMakeFiles/test3.dir/Poller.cpp.o" \
"CMakeFiles/test3.dir/Channel.cpp.o" \
"CMakeFiles/test3.dir/test3.cpp.o"

# External object files for target test3
test3_EXTERNAL_OBJECTS =

test3: CMakeFiles/test3.dir/Eventloop.cpp.o
test3: CMakeFiles/test3.dir/Poller.cpp.o
test3: CMakeFiles/test3.dir/Channel.cpp.o
test3: CMakeFiles/test3.dir/test3.cpp.o
test3: CMakeFiles/test3.dir/build.make
test3: base/libmuduo_base.a
test3: CMakeFiles/test3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable test3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test3.dir/build: test3

.PHONY : CMakeFiles/test3.dir/build

CMakeFiles/test3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test3.dir/clean

CMakeFiles/test3.dir/depend:
	cd /home/zy/Projects/snippet/reactor/s01/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zy/Projects/snippet/reactor/s01 /home/zy/Projects/snippet/reactor/s01 /home/zy/Projects/snippet/reactor/s01/cmake-build-debug /home/zy/Projects/snippet/reactor/s01/cmake-build-debug /home/zy/Projects/snippet/reactor/s01/cmake-build-debug/CMakeFiles/test3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test3.dir/depend

