# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release

# Include any dependencies generated for this target.
include CMakeFiles/minigzip.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/minigzip.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/minigzip.dir/flags.make

CMakeFiles/minigzip.dir/test/minigzip.o: CMakeFiles/minigzip.dir/flags.make
CMakeFiles/minigzip.dir/test/minigzip.o: ../test/minigzip.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/minigzip.dir/test/minigzip.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/minigzip.dir/test/minigzip.o   -c /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/test/minigzip.c

CMakeFiles/minigzip.dir/test/minigzip.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minigzip.dir/test/minigzip.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/test/minigzip.c > CMakeFiles/minigzip.dir/test/minigzip.i

CMakeFiles/minigzip.dir/test/minigzip.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minigzip.dir/test/minigzip.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/test/minigzip.c -o CMakeFiles/minigzip.dir/test/minigzip.s

CMakeFiles/minigzip.dir/test/minigzip.o.requires:

.PHONY : CMakeFiles/minigzip.dir/test/minigzip.o.requires

CMakeFiles/minigzip.dir/test/minigzip.o.provides: CMakeFiles/minigzip.dir/test/minigzip.o.requires
	$(MAKE) -f CMakeFiles/minigzip.dir/build.make CMakeFiles/minigzip.dir/test/minigzip.o.provides.build
.PHONY : CMakeFiles/minigzip.dir/test/minigzip.o.provides

CMakeFiles/minigzip.dir/test/minigzip.o.provides.build: CMakeFiles/minigzip.dir/test/minigzip.o


# Object files for target minigzip
minigzip_OBJECTS = \
"CMakeFiles/minigzip.dir/test/minigzip.o"

# External object files for target minigzip
minigzip_EXTERNAL_OBJECTS =

minigzip: CMakeFiles/minigzip.dir/test/minigzip.o
minigzip: CMakeFiles/minigzip.dir/build.make
minigzip: libz.so.1.2.11
minigzip: CMakeFiles/minigzip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable minigzip"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minigzip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/minigzip.dir/build: minigzip

.PHONY : CMakeFiles/minigzip.dir/build

CMakeFiles/minigzip.dir/requires: CMakeFiles/minigzip.dir/test/minigzip.o.requires

.PHONY : CMakeFiles/minigzip.dir/requires

CMakeFiles/minigzip.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/minigzip.dir/cmake_clean.cmake
.PHONY : CMakeFiles/minigzip.dir/clean

CMakeFiles/minigzip.dir/depend:
	cd /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11 /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11 /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release /home/troy/UBG/Source/UBGEngine/3rdParty/zlib-1.2.11/Release/CMakeFiles/minigzip.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/minigzip.dir/depend
