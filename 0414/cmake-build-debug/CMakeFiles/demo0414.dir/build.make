# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/demo0414.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/demo0414.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/demo0414.dir/flags.make

CMakeFiles/demo0414.dir/main.cpp.obj: CMakeFiles/demo0414.dir/flags.make
CMakeFiles/demo0414.dir/main.cpp.obj: CMakeFiles/demo0414.dir/includes_CXX.rsp
CMakeFiles/demo0414.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/demo0414.dir/main.cpp.obj"
	D:\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\demo0414.dir\main.cpp.obj -c C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\main.cpp

CMakeFiles/demo0414.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo0414.dir/main.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\main.cpp > CMakeFiles\demo0414.dir\main.cpp.i

CMakeFiles/demo0414.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo0414.dir/main.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\main.cpp -o CMakeFiles\demo0414.dir\main.cpp.s

# Object files for target demo0414
demo0414_OBJECTS = \
"CMakeFiles/demo0414.dir/main.cpp.obj"

# External object files for target demo0414
demo0414_EXTERNAL_OBJECTS =

demo0414.exe: CMakeFiles/demo0414.dir/main.cpp.obj
demo0414.exe: CMakeFiles/demo0414.dir/build.make
demo0414.exe: ../extern/lib/win64/microsoft/libmex.lib
demo0414.exe: ../extern/lib/win64/microsoft/libmx.lib
demo0414.exe: ../extern/lib/win64/microsoft/libmat.lib
demo0414.exe: ../extern/lib/win64/microsoft/libeng.lib
demo0414.exe: ../extern/lib/win64/microsoft/mclmcr.lib
demo0414.exe: ../extern/lib/win64/microsoft/mclmcrrt.lib
demo0414.exe: CMakeFiles/demo0414.dir/linklibs.rsp
demo0414.exe: CMakeFiles/demo0414.dir/objects1.rsp
demo0414.exe: CMakeFiles/demo0414.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable demo0414.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\demo0414.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/demo0414.dir/build: demo0414.exe

.PHONY : CMakeFiles/demo0414.dir/build

CMakeFiles/demo0414.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\demo0414.dir\cmake_clean.cmake
.PHONY : CMakeFiles/demo0414.dir/clean

CMakeFiles/demo0414.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414 C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414 C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug C:\Users\Orel\Desktop\CommunicationSoftware_wx\0414\cmake-build-debug\CMakeFiles\demo0414.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/demo0414.dir/depend

