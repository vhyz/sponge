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
include examples/chat/CMakeFiles/chatClient.dir/depend.make

# Include the progress variables for this target.
include examples/chat/CMakeFiles/chatClient.dir/progress.make

# Include the compile flags for this target's objects.
include examples/chat/CMakeFiles/chatClient.dir/flags.make

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o: examples/chat/CMakeFiles/chatClient.dir/flags.make
examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o: ../examples/chat/chatClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vhyz/VSCode/sponge/make/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o"
	cd /home/vhyz/VSCode/sponge/make/examples/chat && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatClient.dir/chatClient.cpp.o -c /home/vhyz/VSCode/sponge/examples/chat/chatClient.cpp

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatClient.dir/chatClient.cpp.i"
	cd /home/vhyz/VSCode/sponge/make/examples/chat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vhyz/VSCode/sponge/examples/chat/chatClient.cpp > CMakeFiles/chatClient.dir/chatClient.cpp.i

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatClient.dir/chatClient.cpp.s"
	cd /home/vhyz/VSCode/sponge/make/examples/chat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vhyz/VSCode/sponge/examples/chat/chatClient.cpp -o CMakeFiles/chatClient.dir/chatClient.cpp.s

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.requires:

.PHONY : examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.requires

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.provides: examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.requires
	$(MAKE) -f examples/chat/CMakeFiles/chatClient.dir/build.make examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.provides.build
.PHONY : examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.provides

examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.provides.build: examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o


# Object files for target chatClient
chatClient_OBJECTS = \
"CMakeFiles/chatClient.dir/chatClient.cpp.o"

# External object files for target chatClient
chatClient_EXTERNAL_OBJECTS =

examples/chat/chatClient: examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o
examples/chat/chatClient: examples/chat/CMakeFiles/chatClient.dir/build.make
examples/chat/chatClient: sponge/libsponge.a
examples/chat/chatClient: sponge/http/libsponge_http.a
examples/chat/chatClient: examples/chat/CMakeFiles/chatClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vhyz/VSCode/sponge/make/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable chatClient"
	cd /home/vhyz/VSCode/sponge/make/examples/chat && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/chat/CMakeFiles/chatClient.dir/build: examples/chat/chatClient

.PHONY : examples/chat/CMakeFiles/chatClient.dir/build

examples/chat/CMakeFiles/chatClient.dir/requires: examples/chat/CMakeFiles/chatClient.dir/chatClient.cpp.o.requires

.PHONY : examples/chat/CMakeFiles/chatClient.dir/requires

examples/chat/CMakeFiles/chatClient.dir/clean:
	cd /home/vhyz/VSCode/sponge/make/examples/chat && $(CMAKE_COMMAND) -P CMakeFiles/chatClient.dir/cmake_clean.cmake
.PHONY : examples/chat/CMakeFiles/chatClient.dir/clean

examples/chat/CMakeFiles/chatClient.dir/depend:
	cd /home/vhyz/VSCode/sponge/make && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vhyz/VSCode/sponge /home/vhyz/VSCode/sponge/examples/chat /home/vhyz/VSCode/sponge/make /home/vhyz/VSCode/sponge/make/examples/chat /home/vhyz/VSCode/sponge/make/examples/chat/CMakeFiles/chatClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/chat/CMakeFiles/chatClient.dir/depend
