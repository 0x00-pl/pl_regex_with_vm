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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pl/Documents/kdevelop/plre

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pl/Documents/kdevelop/plre/build

# Include any dependencies generated for this target.
include CMakeFiles/plre.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/plre.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/plre.dir/flags.make

CMakeFiles/plre.dir/test.cpp.o: CMakeFiles/plre.dir/flags.make
CMakeFiles/plre.dir/test.cpp.o: ../test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pl/Documents/kdevelop/plre/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/plre.dir/test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/plre.dir/test.cpp.o -c /home/pl/Documents/kdevelop/plre/test.cpp

CMakeFiles/plre.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plre.dir/test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pl/Documents/kdevelop/plre/test.cpp > CMakeFiles/plre.dir/test.cpp.i

CMakeFiles/plre.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plre.dir/test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pl/Documents/kdevelop/plre/test.cpp -o CMakeFiles/plre.dir/test.cpp.s

CMakeFiles/plre.dir/test.cpp.o.requires:
.PHONY : CMakeFiles/plre.dir/test.cpp.o.requires

CMakeFiles/plre.dir/test.cpp.o.provides: CMakeFiles/plre.dir/test.cpp.o.requires
	$(MAKE) -f CMakeFiles/plre.dir/build.make CMakeFiles/plre.dir/test.cpp.o.provides.build
.PHONY : CMakeFiles/plre.dir/test.cpp.o.provides

CMakeFiles/plre.dir/test.cpp.o.provides.build: CMakeFiles/plre.dir/test.cpp.o

CMakeFiles/plre.dir/src/ast.cpp.o: CMakeFiles/plre.dir/flags.make
CMakeFiles/plre.dir/src/ast.cpp.o: ../src/ast.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pl/Documents/kdevelop/plre/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/plre.dir/src/ast.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/plre.dir/src/ast.cpp.o -c /home/pl/Documents/kdevelop/plre/src/ast.cpp

CMakeFiles/plre.dir/src/ast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plre.dir/src/ast.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pl/Documents/kdevelop/plre/src/ast.cpp > CMakeFiles/plre.dir/src/ast.cpp.i

CMakeFiles/plre.dir/src/ast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plre.dir/src/ast.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pl/Documents/kdevelop/plre/src/ast.cpp -o CMakeFiles/plre.dir/src/ast.cpp.s

CMakeFiles/plre.dir/src/ast.cpp.o.requires:
.PHONY : CMakeFiles/plre.dir/src/ast.cpp.o.requires

CMakeFiles/plre.dir/src/ast.cpp.o.provides: CMakeFiles/plre.dir/src/ast.cpp.o.requires
	$(MAKE) -f CMakeFiles/plre.dir/build.make CMakeFiles/plre.dir/src/ast.cpp.o.provides.build
.PHONY : CMakeFiles/plre.dir/src/ast.cpp.o.provides

CMakeFiles/plre.dir/src/ast.cpp.o.provides.build: CMakeFiles/plre.dir/src/ast.cpp.o

CMakeFiles/plre.dir/src/str_convert.cpp.o: CMakeFiles/plre.dir/flags.make
CMakeFiles/plre.dir/src/str_convert.cpp.o: ../src/str_convert.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pl/Documents/kdevelop/plre/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/plre.dir/src/str_convert.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/plre.dir/src/str_convert.cpp.o -c /home/pl/Documents/kdevelop/plre/src/str_convert.cpp

CMakeFiles/plre.dir/src/str_convert.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plre.dir/src/str_convert.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pl/Documents/kdevelop/plre/src/str_convert.cpp > CMakeFiles/plre.dir/src/str_convert.cpp.i

CMakeFiles/plre.dir/src/str_convert.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plre.dir/src/str_convert.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pl/Documents/kdevelop/plre/src/str_convert.cpp -o CMakeFiles/plre.dir/src/str_convert.cpp.s

CMakeFiles/plre.dir/src/str_convert.cpp.o.requires:
.PHONY : CMakeFiles/plre.dir/src/str_convert.cpp.o.requires

CMakeFiles/plre.dir/src/str_convert.cpp.o.provides: CMakeFiles/plre.dir/src/str_convert.cpp.o.requires
	$(MAKE) -f CMakeFiles/plre.dir/build.make CMakeFiles/plre.dir/src/str_convert.cpp.o.provides.build
.PHONY : CMakeFiles/plre.dir/src/str_convert.cpp.o.provides

CMakeFiles/plre.dir/src/str_convert.cpp.o.provides.build: CMakeFiles/plre.dir/src/str_convert.cpp.o

# Object files for target plre
plre_OBJECTS = \
"CMakeFiles/plre.dir/test.cpp.o" \
"CMakeFiles/plre.dir/src/ast.cpp.o" \
"CMakeFiles/plre.dir/src/str_convert.cpp.o"

# External object files for target plre
plre_EXTERNAL_OBJECTS =

plre: CMakeFiles/plre.dir/test.cpp.o
plre: CMakeFiles/plre.dir/src/ast.cpp.o
plre: CMakeFiles/plre.dir/src/str_convert.cpp.o
plre: CMakeFiles/plre.dir/build.make
plre: CMakeFiles/plre.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable plre"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/plre.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/plre.dir/build: plre
.PHONY : CMakeFiles/plre.dir/build

CMakeFiles/plre.dir/requires: CMakeFiles/plre.dir/test.cpp.o.requires
CMakeFiles/plre.dir/requires: CMakeFiles/plre.dir/src/ast.cpp.o.requires
CMakeFiles/plre.dir/requires: CMakeFiles/plre.dir/src/str_convert.cpp.o.requires
.PHONY : CMakeFiles/plre.dir/requires

CMakeFiles/plre.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/plre.dir/cmake_clean.cmake
.PHONY : CMakeFiles/plre.dir/clean

CMakeFiles/plre.dir/depend:
	cd /home/pl/Documents/kdevelop/plre/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pl/Documents/kdevelop/plre /home/pl/Documents/kdevelop/plre /home/pl/Documents/kdevelop/plre/build /home/pl/Documents/kdevelop/plre/build /home/pl/Documents/kdevelop/plre/build/CMakeFiles/plre.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/plre.dir/depend

