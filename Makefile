# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-12.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gregorybarber/Desktop/final_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gregorybarber/Desktop/final_project

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"/Applications/CMake 2.8-12.app/Contents/bin/ccmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"/Applications/CMake 2.8-12.app/Contents/bin/cmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/gregorybarber/Desktop/final_project/CMakeFiles /Users/gregorybarber/Desktop/final_project/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/gregorybarber/Desktop/final_project/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named pa3

# Build rule for target.
pa3: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 pa3
.PHONY : pa3

# fast build rule for target.
pa3/fast:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/build
.PHONY : pa3/fast

src/GLSLProgram.o: src/GLSLProgram.cpp.o
.PHONY : src/GLSLProgram.o

# target to build an object file
src/GLSLProgram.cpp.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLSLProgram.cpp.o
.PHONY : src/GLSLProgram.cpp.o

src/GLSLProgram.i: src/GLSLProgram.cpp.i
.PHONY : src/GLSLProgram.i

# target to preprocess a source file
src/GLSLProgram.cpp.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLSLProgram.cpp.i
.PHONY : src/GLSLProgram.cpp.i

src/GLSLProgram.s: src/GLSLProgram.cpp.s
.PHONY : src/GLSLProgram.s

# target to generate assembly for a file
src/GLSLProgram.cpp.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLSLProgram.cpp.s
.PHONY : src/GLSLProgram.cpp.s

src/GLScreenCapturer.o: src/GLScreenCapturer.cpp.o
.PHONY : src/GLScreenCapturer.o

# target to build an object file
src/GLScreenCapturer.cpp.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLScreenCapturer.cpp.o
.PHONY : src/GLScreenCapturer.cpp.o

src/GLScreenCapturer.i: src/GLScreenCapturer.cpp.i
.PHONY : src/GLScreenCapturer.i

# target to preprocess a source file
src/GLScreenCapturer.cpp.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLScreenCapturer.cpp.i
.PHONY : src/GLScreenCapturer.cpp.i

src/GLScreenCapturer.s: src/GLScreenCapturer.cpp.s
.PHONY : src/GLScreenCapturer.s

# target to generate assembly for a file
src/GLScreenCapturer.cpp.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/GLScreenCapturer.cpp.s
.PHONY : src/GLScreenCapturer.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/soil/SOIL.o: src/soil/SOIL.c.o
.PHONY : src/soil/SOIL.o

# target to build an object file
src/soil/SOIL.c.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/SOIL.c.o
.PHONY : src/soil/SOIL.c.o

src/soil/SOIL.i: src/soil/SOIL.c.i
.PHONY : src/soil/SOIL.i

# target to preprocess a source file
src/soil/SOIL.c.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/SOIL.c.i
.PHONY : src/soil/SOIL.c.i

src/soil/SOIL.s: src/soil/SOIL.c.s
.PHONY : src/soil/SOIL.s

# target to generate assembly for a file
src/soil/SOIL.c.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/SOIL.c.s
.PHONY : src/soil/SOIL.c.s

src/soil/image_DXT.o: src/soil/image_DXT.c.o
.PHONY : src/soil/image_DXT.o

# target to build an object file
src/soil/image_DXT.c.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_DXT.c.o
.PHONY : src/soil/image_DXT.c.o

src/soil/image_DXT.i: src/soil/image_DXT.c.i
.PHONY : src/soil/image_DXT.i

# target to preprocess a source file
src/soil/image_DXT.c.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_DXT.c.i
.PHONY : src/soil/image_DXT.c.i

src/soil/image_DXT.s: src/soil/image_DXT.c.s
.PHONY : src/soil/image_DXT.s

# target to generate assembly for a file
src/soil/image_DXT.c.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_DXT.c.s
.PHONY : src/soil/image_DXT.c.s

src/soil/image_helper.o: src/soil/image_helper.c.o
.PHONY : src/soil/image_helper.o

# target to build an object file
src/soil/image_helper.c.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_helper.c.o
.PHONY : src/soil/image_helper.c.o

src/soil/image_helper.i: src/soil/image_helper.c.i
.PHONY : src/soil/image_helper.i

# target to preprocess a source file
src/soil/image_helper.c.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_helper.c.i
.PHONY : src/soil/image_helper.c.i

src/soil/image_helper.s: src/soil/image_helper.c.s
.PHONY : src/soil/image_helper.s

# target to generate assembly for a file
src/soil/image_helper.c.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/image_helper.c.s
.PHONY : src/soil/image_helper.c.s

src/soil/stb_image_aug.o: src/soil/stb_image_aug.c.o
.PHONY : src/soil/stb_image_aug.o

# target to build an object file
src/soil/stb_image_aug.c.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/stb_image_aug.c.o
.PHONY : src/soil/stb_image_aug.c.o

src/soil/stb_image_aug.i: src/soil/stb_image_aug.c.i
.PHONY : src/soil/stb_image_aug.i

# target to preprocess a source file
src/soil/stb_image_aug.c.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/stb_image_aug.c.i
.PHONY : src/soil/stb_image_aug.c.i

src/soil/stb_image_aug.s: src/soil/stb_image_aug.c.s
.PHONY : src/soil/stb_image_aug.s

# target to generate assembly for a file
src/soil/stb_image_aug.c.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/soil/stb_image_aug.c.s
.PHONY : src/soil/stb_image_aug.c.s

src/trackball.o: src/trackball.cpp.o
.PHONY : src/trackball.o

# target to build an object file
src/trackball.cpp.o:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/trackball.cpp.o
.PHONY : src/trackball.cpp.o

src/trackball.i: src/trackball.cpp.i
.PHONY : src/trackball.i

# target to preprocess a source file
src/trackball.cpp.i:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/trackball.cpp.i
.PHONY : src/trackball.cpp.i

src/trackball.s: src/trackball.cpp.s
.PHONY : src/trackball.s

# target to generate assembly for a file
src/trackball.cpp.s:
	$(MAKE) -f CMakeFiles/pa3.dir/build.make CMakeFiles/pa3.dir/src/trackball.cpp.s
.PHONY : src/trackball.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... pa3"
	@echo "... rebuild_cache"
	@echo "... src/GLSLProgram.o"
	@echo "... src/GLSLProgram.i"
	@echo "... src/GLSLProgram.s"
	@echo "... src/GLScreenCapturer.o"
	@echo "... src/GLScreenCapturer.i"
	@echo "... src/GLScreenCapturer.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/soil/SOIL.o"
	@echo "... src/soil/SOIL.i"
	@echo "... src/soil/SOIL.s"
	@echo "... src/soil/image_DXT.o"
	@echo "... src/soil/image_DXT.i"
	@echo "... src/soil/image_DXT.s"
	@echo "... src/soil/image_helper.o"
	@echo "... src/soil/image_helper.i"
	@echo "... src/soil/image_helper.s"
	@echo "... src/soil/stb_image_aug.o"
	@echo "... src/soil/stb_image_aug.i"
	@echo "... src/soil/stb_image_aug.s"
	@echo "... src/trackball.o"
	@echo "... src/trackball.i"
	@echo "... src/trackball.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

