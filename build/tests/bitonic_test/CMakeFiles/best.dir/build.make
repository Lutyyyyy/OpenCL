# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luty/proga/Vlados/OpenCl/bitonic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luty/proga/Vlados/OpenCl/bitonic/build

# Include any dependencies generated for this target.
include tests/bitonic_test/CMakeFiles/best.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/bitonic_test/CMakeFiles/best.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/bitonic_test/CMakeFiles/best.dir/progress.make

# Include the compile flags for this target's objects.
include tests/bitonic_test/CMakeFiles/best.dir/flags.make

tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o: tests/bitonic_test/CMakeFiles/best.dir/flags.make
tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o: ../tests/bitonic_test/btest.cpp
tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o: tests/bitonic_test/CMakeFiles/best.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luty/proga/Vlados/OpenCl/bitonic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o"
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o -MF CMakeFiles/best.dir/btest.cpp.o.d -o CMakeFiles/best.dir/btest.cpp.o -c /home/luty/proga/Vlados/OpenCl/bitonic/tests/bitonic_test/btest.cpp

tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/best.dir/btest.cpp.i"
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luty/proga/Vlados/OpenCl/bitonic/tests/bitonic_test/btest.cpp > CMakeFiles/best.dir/btest.cpp.i

tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/best.dir/btest.cpp.s"
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luty/proga/Vlados/OpenCl/bitonic/tests/bitonic_test/btest.cpp -o CMakeFiles/best.dir/btest.cpp.s

# Object files for target best
best_OBJECTS = \
"CMakeFiles/best.dir/btest.cpp.o"

# External object files for target best
best_EXTERNAL_OBJECTS =

tests/bitonic_test/best: tests/bitonic_test/CMakeFiles/best.dir/btest.cpp.o
tests/bitonic_test/best: tests/bitonic_test/CMakeFiles/best.dir/build.make
tests/bitonic_test/best: tests/bitonic_test/CMakeFiles/best.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luty/proga/Vlados/OpenCl/bitonic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable best"
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/best.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/bitonic_test/CMakeFiles/best.dir/build: tests/bitonic_test/best
.PHONY : tests/bitonic_test/CMakeFiles/best.dir/build

tests/bitonic_test/CMakeFiles/best.dir/clean:
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test && $(CMAKE_COMMAND) -P CMakeFiles/best.dir/cmake_clean.cmake
.PHONY : tests/bitonic_test/CMakeFiles/best.dir/clean

tests/bitonic_test/CMakeFiles/best.dir/depend:
	cd /home/luty/proga/Vlados/OpenCl/bitonic/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luty/proga/Vlados/OpenCl/bitonic /home/luty/proga/Vlados/OpenCl/bitonic/tests/bitonic_test /home/luty/proga/Vlados/OpenCl/bitonic/build /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test /home/luty/proga/Vlados/OpenCl/bitonic/build/tests/bitonic_test/CMakeFiles/best.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/bitonic_test/CMakeFiles/best.dir/depend

