# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\Apps\CLion\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = D:\Apps\CLion\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Project\Clion Project\My_STLite"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Project\Clion Project\My_STLite\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/My_STLite.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/My_STLite.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/My_STLite.dir/flags.make

CMakeFiles/My_STLite.dir/main.cpp.obj: CMakeFiles/My_STLite.dir/flags.make
CMakeFiles/My_STLite.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Project\Clion Project\My_STLite\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/My_STLite.dir/main.cpp.obj"
	D:\Apps\MinGW\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\My_STLite.dir\main.cpp.obj -c "D:\Project\Clion Project\My_STLite\main.cpp"

CMakeFiles/My_STLite.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/My_STLite.dir/main.cpp.i"
	D:\Apps\MinGW\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\Clion Project\My_STLite\main.cpp" > CMakeFiles\My_STLite.dir\main.cpp.i

CMakeFiles/My_STLite.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/My_STLite.dir/main.cpp.s"
	D:\Apps\MinGW\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\Clion Project\My_STLite\main.cpp" -o CMakeFiles\My_STLite.dir\main.cpp.s

# Object files for target My_STLite
My_STLite_OBJECTS = \
"CMakeFiles/My_STLite.dir/main.cpp.obj"

# External object files for target My_STLite
My_STLite_EXTERNAL_OBJECTS =

My_STLite.exe: CMakeFiles/My_STLite.dir/main.cpp.obj
My_STLite.exe: CMakeFiles/My_STLite.dir/build.make
My_STLite.exe: CMakeFiles/My_STLite.dir/linklibs.rsp
My_STLite.exe: CMakeFiles/My_STLite.dir/objects1.rsp
My_STLite.exe: CMakeFiles/My_STLite.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\Project\Clion Project\My_STLite\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable My_STLite.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\My_STLite.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/My_STLite.dir/build: My_STLite.exe

.PHONY : CMakeFiles/My_STLite.dir/build

CMakeFiles/My_STLite.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\My_STLite.dir\cmake_clean.cmake
.PHONY : CMakeFiles/My_STLite.dir/clean

CMakeFiles/My_STLite.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Project\Clion Project\My_STLite" "D:\Project\Clion Project\My_STLite" "D:\Project\Clion Project\My_STLite\cmake-build-debug" "D:\Project\Clion Project\My_STLite\cmake-build-debug" "D:\Project\Clion Project\My_STLite\cmake-build-debug\CMakeFiles\My_STLite.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/My_STLite.dir/depend

