# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Kamil\Desktop\Server_git

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Kamil\Desktop\Server_git\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Server_2.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Server_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Server_2.dir/flags.make

CMakeFiles/Server_2.dir/main.c.obj: CMakeFiles/Server_2.dir/flags.make
CMakeFiles/Server_2.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Server_2.dir/main.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server_2.dir\main.c.obj -c C:\Users\Kamil\Desktop\Server_git\main.c

CMakeFiles/Server_2.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server_2.dir/main.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Kamil\Desktop\Server_git\main.c > CMakeFiles\Server_2.dir\main.c.i

CMakeFiles/Server_2.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server_2.dir/main.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Kamil\Desktop\Server_git\main.c -o CMakeFiles\Server_2.dir\main.c.s

CMakeFiles/Server_2.dir/create_array.c.obj: CMakeFiles/Server_2.dir/flags.make
CMakeFiles/Server_2.dir/create_array.c.obj: ../create_array.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Server_2.dir/create_array.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server_2.dir\create_array.c.obj -c C:\Users\Kamil\Desktop\Server_git\create_array.c

CMakeFiles/Server_2.dir/create_array.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server_2.dir/create_array.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Kamil\Desktop\Server_git\create_array.c > CMakeFiles\Server_2.dir\create_array.c.i

CMakeFiles/Server_2.dir/create_array.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server_2.dir/create_array.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Kamil\Desktop\Server_git\create_array.c -o CMakeFiles\Server_2.dir\create_array.c.s

CMakeFiles/Server_2.dir/buffer.c.obj: CMakeFiles/Server_2.dir/flags.make
CMakeFiles/Server_2.dir/buffer.c.obj: ../buffer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Server_2.dir/buffer.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server_2.dir\buffer.c.obj -c C:\Users\Kamil\Desktop\Server_git\buffer.c

CMakeFiles/Server_2.dir/buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server_2.dir/buffer.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Kamil\Desktop\Server_git\buffer.c > CMakeFiles\Server_2.dir\buffer.c.i

CMakeFiles/Server_2.dir/buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server_2.dir/buffer.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Kamil\Desktop\Server_git\buffer.c -o CMakeFiles\Server_2.dir\buffer.c.s

CMakeFiles/Server_2.dir/beast.c.obj: CMakeFiles/Server_2.dir/flags.make
CMakeFiles/Server_2.dir/beast.c.obj: ../beast.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Server_2.dir/beast.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server_2.dir\beast.c.obj -c C:\Users\Kamil\Desktop\Server_git\beast.c

CMakeFiles/Server_2.dir/beast.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server_2.dir/beast.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Kamil\Desktop\Server_git\beast.c > CMakeFiles\Server_2.dir\beast.c.i

CMakeFiles/Server_2.dir/beast.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server_2.dir/beast.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Kamil\Desktop\Server_git\beast.c -o CMakeFiles\Server_2.dir\beast.c.s

# Object files for target Server_2
Server_2_OBJECTS = \
"CMakeFiles/Server_2.dir/main.c.obj" \
"CMakeFiles/Server_2.dir/create_array.c.obj" \
"CMakeFiles/Server_2.dir/buffer.c.obj" \
"CMakeFiles/Server_2.dir/beast.c.obj"

# External object files for target Server_2
Server_2_EXTERNAL_OBJECTS =

Server_2.exe: CMakeFiles/Server_2.dir/main.c.obj
Server_2.exe: CMakeFiles/Server_2.dir/create_array.c.obj
Server_2.exe: CMakeFiles/Server_2.dir/buffer.c.obj
Server_2.exe: CMakeFiles/Server_2.dir/beast.c.obj
Server_2.exe: CMakeFiles/Server_2.dir/build.make
Server_2.exe: CMakeFiles/Server_2.dir/linklibs.rsp
Server_2.exe: CMakeFiles/Server_2.dir/objects1.rsp
Server_2.exe: CMakeFiles/Server_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable Server_2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Server_2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Server_2.dir/build: Server_2.exe
.PHONY : CMakeFiles/Server_2.dir/build

CMakeFiles/Server_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Server_2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Server_2.dir/clean

CMakeFiles/Server_2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kamil\Desktop\Server_git C:\Users\Kamil\Desktop\Server_git C:\Users\Kamil\Desktop\Server_git\cmake-build-debug C:\Users\Kamil\Desktop\Server_git\cmake-build-debug C:\Users\Kamil\Desktop\Server_git\cmake-build-debug\CMakeFiles\Server_2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Server_2.dir/depend

