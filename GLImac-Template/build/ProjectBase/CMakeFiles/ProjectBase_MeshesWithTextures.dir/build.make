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
CMAKE_SOURCE_DIR = /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build

# Include any dependencies generated for this target.
include ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/depend.make

# Include the progress variables for this target.
include ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/progress.make

# Include the compile flags for this target's objects.
include ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/flags.make

ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o: ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/flags.make
ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o: ../ProjectBase/MeshesWithTextures.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o"
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o -c /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/ProjectBase/MeshesWithTextures.cpp

ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.i"
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/ProjectBase/MeshesWithTextures.cpp > CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.i

ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.s"
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/ProjectBase/MeshesWithTextures.cpp -o CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.s

# Object files for target ProjectBase_MeshesWithTextures
ProjectBase_MeshesWithTextures_OBJECTS = \
"CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o"

# External object files for target ProjectBase_MeshesWithTextures
ProjectBase_MeshesWithTextures_EXTERNAL_OBJECTS =

ProjectBase/ProjectBase_MeshesWithTextures: ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/MeshesWithTextures.cpp.o
ProjectBase/ProjectBase_MeshesWithTextures: ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/build.make
ProjectBase/ProjectBase_MeshesWithTextures: glimac/libglimac.a
ProjectBase/ProjectBase_MeshesWithTextures: /usr/lib/x86_64-linux-gnu/libSDLmain.a
ProjectBase/ProjectBase_MeshesWithTextures: /usr/lib/x86_64-linux-gnu/libSDL.so
ProjectBase/ProjectBase_MeshesWithTextures: /usr/lib/x86_64-linux-gnu/libGL.so.1
ProjectBase/ProjectBase_MeshesWithTextures: ../third-party/libGLEW.a
ProjectBase/ProjectBase_MeshesWithTextures: ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ProjectBase_MeshesWithTextures"
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ProjectBase_MeshesWithTextures.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/build: ProjectBase/ProjectBase_MeshesWithTextures

.PHONY : ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/build

ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/clean:
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase && $(CMAKE_COMMAND) -P CMakeFiles/ProjectBase_MeshesWithTextures.dir/cmake_clean.cmake
.PHONY : ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/clean

ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/depend:
	cd /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/ProjectBase /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase /mnt/c/Users/huiyi/OneDrive/Documents/E5/OpenGL/opengl_projet/GLImac-Template/build/ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ProjectBase/CMakeFiles/ProjectBase_MeshesWithTextures.dir/depend
