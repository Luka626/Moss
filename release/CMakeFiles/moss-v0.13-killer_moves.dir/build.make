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
CMAKE_SOURCE_DIR = /home/luka/Moss

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luka/Moss/release

# Include any dependencies generated for this target.
include CMakeFiles/moss-v0.13-killer_moves.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/moss-v0.13-killer_moves.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/moss-v0.13-killer_moves.dir/flags.make

CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o -c /home/luka/Moss/src/main.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/main.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/main.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o: ../src/move_generator.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o -c /home/luka/Moss/src/move_generator.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/move_generator.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/move_generator.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o: ../src/position.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o -c /home/luka/Moss/src/position.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/position.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/position.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o: ../src/utils.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o -c /home/luka/Moss/src/utils.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/utils.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/utils.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o: ../src/uci.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o -c /home/luka/Moss/src/uci.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/uci.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/uci.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o: ../src/search.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o -c /home/luka/Moss/src/search.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/search.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/search.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o: ../src/eval.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o -c /home/luka/Moss/src/eval.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/eval.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/eval.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o: ../src/zobrist.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o -c /home/luka/Moss/src/zobrist.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/zobrist.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/zobrist.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.s

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/flags.make
CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o: ../src/move_list.cpp
CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o: CMakeFiles/moss-v0.13-killer_moves.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o -MF CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o.d -o CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o -c /home/luka/Moss/src/move_list.cpp

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Moss/src/move_list.cpp > CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.i

CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Moss/src/move_list.cpp -o CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.s

# Object files for target moss-v0.13-killer_moves
moss__v0_13__killer_moves_OBJECTS = \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o" \
"CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o"

# External object files for target moss-v0.13-killer_moves
moss__v0_13__killer_moves_EXTERNAL_OBJECTS =

../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/main.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/move_generator.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/position.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/utils.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/uci.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/search.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/eval.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/zobrist.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/src/move_list.cpp.o
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/build.make
../bin/moss-v0.13-killer_moves-0.13.1: CMakeFiles/moss-v0.13-killer_moves.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luka/Moss/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable ../bin/moss-v0.13-killer_moves"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/moss-v0.13-killer_moves.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_executable ../bin/moss-v0.13-killer_moves-0.13.1 ../bin/moss-v0.13-killer_moves

../bin/moss-v0.13-killer_moves: ../bin/moss-v0.13-killer_moves-0.13.1

# Rule to build all files generated by this target.
CMakeFiles/moss-v0.13-killer_moves.dir/build: ../bin/moss-v0.13-killer_moves
.PHONY : CMakeFiles/moss-v0.13-killer_moves.dir/build

CMakeFiles/moss-v0.13-killer_moves.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/moss-v0.13-killer_moves.dir/cmake_clean.cmake
.PHONY : CMakeFiles/moss-v0.13-killer_moves.dir/clean

CMakeFiles/moss-v0.13-killer_moves.dir/depend:
	cd /home/luka/Moss/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luka/Moss /home/luka/Moss /home/luka/Moss/release /home/luka/Moss/release /home/luka/Moss/release/CMakeFiles/moss-v0.13-killer_moves.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/moss-v0.13-killer_moves.dir/depend

