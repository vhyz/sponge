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
CMAKE_SOURCE_DIR = /home/vhyz/VSCode/sponge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vhyz/VSCode/sponge/make

# Include any dependencies generated for this target.
include tests/CMakeFiles/TimerQueue_test.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/TimerQueue_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/TimerQueue_test.dir/flags.make

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o: tests/CMakeFiles/TimerQueue_test.dir/flags.make
tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o: ../tests/TimerQueue_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vhyz/VSCode/sponge/make/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o"
	cd /home/vhyz/VSCode/sponge/make/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o -c /home/vhyz/VSCode/sponge/tests/TimerQueue_test.cpp

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.i"
	cd /home/vhyz/VSCode/sponge/make/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vhyz/VSCode/sponge/tests/TimerQueue_test.cpp > CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.i

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.s"
	cd /home/vhyz/VSCode/sponge/make/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vhyz/VSCode/sponge/tests/TimerQueue_test.cpp -o CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.s

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.requires:

.PHONY : tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.requires

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.provides: tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/TimerQueue_test.dir/build.make tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.provides.build
.PHONY : tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.provides

tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.provides.build: tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o


# Object files for target TimerQueue_test
TimerQueue_test_OBJECTS = \
"CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o"

# External object files for target TimerQueue_test
TimerQueue_test_EXTERNAL_OBJECTS =

tests/TimerQueue_test: tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o
tests/TimerQueue_test: tests/CMakeFiles/TimerQueue_test.dir/build.make
tests/TimerQueue_test: sponge/libsponge.a
tests/TimerQueue_test: sponge/http/libsponge_http.a
tests/TimerQueue_test: tests/CMakeFiles/TimerQueue_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vhyz/VSCode/sponge/make/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TimerQueue_test"
	cd /home/vhyz/VSCode/sponge/make/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TimerQueue_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/TimerQueue_test.dir/build: tests/TimerQueue_test

.PHONY : tests/CMakeFiles/TimerQueue_test.dir/build

tests/CMakeFiles/TimerQueue_test.dir/requires: tests/CMakeFiles/TimerQueue_test.dir/TimerQueue_test.cpp.o.requires

.PHONY : tests/CMakeFiles/TimerQueue_test.dir/requires

tests/CMakeFiles/TimerQueue_test.dir/clean:
	cd /home/vhyz/VSCode/sponge/make/tests && $(CMAKE_COMMAND) -P CMakeFiles/TimerQueue_test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/TimerQueue_test.dir/clean

tests/CMakeFiles/TimerQueue_test.dir/depend:
	cd /home/vhyz/VSCode/sponge/make && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vhyz/VSCode/sponge /home/vhyz/VSCode/sponge/tests /home/vhyz/VSCode/sponge/make /home/vhyz/VSCode/sponge/make/tests /home/vhyz/VSCode/sponge/make/tests/CMakeFiles/TimerQueue_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/TimerQueue_test.dir/depend
