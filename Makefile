# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/swh/codes/ReinBits

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/swh/codes/ReinBits

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/swh/codes/ReinBits/CMakeFiles /home/swh/codes/ReinBits/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/swh/codes/ReinBits/CMakeFiles 0
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
# Target rules for targets named cmakeRB

# Build rule for target.
cmakeRB: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 cmakeRB
.PHONY : cmakeRB

# fast build rule for target.
cmakeRB/fast:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/build
.PHONY : cmakeRB/fast

BIOP.o: BIOP.cpp.o

.PHONY : BIOP.o

# target to build an object file
BIOP.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP.cpp.o
.PHONY : BIOP.cpp.o

BIOP.i: BIOP.cpp.i

.PHONY : BIOP.i

# target to preprocess a source file
BIOP.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP.cpp.i
.PHONY : BIOP.cpp.i

BIOP.s: BIOP.cpp.s

.PHONY : BIOP.s

# target to generate assembly for a file
BIOP.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP.cpp.s
.PHONY : BIOP.cpp.s

BIOP1.o: BIOP1.cpp.o

.PHONY : BIOP1.o

# target to build an object file
BIOP1.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP1.cpp.o
.PHONY : BIOP1.cpp.o

BIOP1.i: BIOP1.cpp.i

.PHONY : BIOP1.i

# target to preprocess a source file
BIOP1.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP1.cpp.i
.PHONY : BIOP1.cpp.i

BIOP1.s: BIOP1.cpp.s

.PHONY : BIOP1.s

# target to generate assembly for a file
BIOP1.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP1.cpp.s
.PHONY : BIOP1.cpp.s

BIOP2.o: BIOP2.cpp.o

.PHONY : BIOP2.o

# target to build an object file
BIOP2.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP2.cpp.o
.PHONY : BIOP2.cpp.o

BIOP2.i: BIOP2.cpp.i

.PHONY : BIOP2.i

# target to preprocess a source file
BIOP2.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP2.cpp.i
.PHONY : BIOP2.cpp.i

BIOP2.s: BIOP2.cpp.s

.PHONY : BIOP2.s

# target to generate assembly for a file
BIOP2.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP2.cpp.s
.PHONY : BIOP2.cpp.s

BIOP3.o: BIOP3.cpp.o

.PHONY : BIOP3.o

# target to build an object file
BIOP3.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP3.cpp.o
.PHONY : BIOP3.cpp.o

BIOP3.i: BIOP3.cpp.i

.PHONY : BIOP3.i

# target to preprocess a source file
BIOP3.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP3.cpp.i
.PHONY : BIOP3.cpp.i

BIOP3.s: BIOP3.cpp.s

.PHONY : BIOP3.s

# target to generate assembly for a file
BIOP3.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP3.cpp.s
.PHONY : BIOP3.cpp.s

BIOP4.o: BIOP4.cpp.o

.PHONY : BIOP4.o

# target to build an object file
BIOP4.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP4.cpp.o
.PHONY : BIOP4.cpp.o

BIOP4.i: BIOP4.cpp.i

.PHONY : BIOP4.i

# target to preprocess a source file
BIOP4.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP4.cpp.i
.PHONY : BIOP4.cpp.i

BIOP4.s: BIOP4.cpp.s

.PHONY : BIOP4.s

# target to generate assembly for a file
BIOP4.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP4.cpp.s
.PHONY : BIOP4.cpp.s

BIOP5.o: BIOP5.cpp.o

.PHONY : BIOP5.o

# target to build an object file
BIOP5.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP5.cpp.o
.PHONY : BIOP5.cpp.o

BIOP5.i: BIOP5.cpp.i

.PHONY : BIOP5.i

# target to preprocess a source file
BIOP5.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP5.cpp.i
.PHONY : BIOP5.cpp.i

BIOP5.s: BIOP5.cpp.s

.PHONY : BIOP5.s

# target to generate assembly for a file
BIOP5.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/BIOP5.cpp.s
.PHONY : BIOP5.cpp.s

constant.o: constant.cpp.o

.PHONY : constant.o

# target to build an object file
constant.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/constant.cpp.o
.PHONY : constant.cpp.o

constant.i: constant.cpp.i

.PHONY : constant.i

# target to preprocess a source file
constant.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/constant.cpp.i
.PHONY : constant.cpp.i

constant.s: constant.cpp.s

.PHONY : constant.s

# target to generate assembly for a file
constant.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/constant.cpp.s
.PHONY : constant.cpp.s

generator.o: generator.cpp.o

.PHONY : generator.o

# target to build an object file
generator.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/generator.cpp.o
.PHONY : generator.cpp.o

generator.i: generator.cpp.i

.PHONY : generator.i

# target to preprocess a source file
generator.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/generator.cpp.i
.PHONY : generator.cpp.i

generator.s: generator.cpp.s

.PHONY : generator.s

# target to generate assembly for a file
generator.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/generator.cpp.s
.PHONY : generator.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/main.cpp.s
.PHONY : main.cpp.s

printer.o: printer.cpp.o

.PHONY : printer.o

# target to build an object file
printer.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/printer.cpp.o
.PHONY : printer.cpp.o

printer.i: printer.cpp.i

.PHONY : printer.i

# target to preprocess a source file
printer.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/printer.cpp.i
.PHONY : printer.cpp.i

printer.s: printer.cpp.s

.PHONY : printer.s

# target to generate assembly for a file
printer.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/printer.cpp.s
.PHONY : printer.cpp.s

rein.o: rein.cpp.o

.PHONY : rein.o

# target to build an object file
rein.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/rein.cpp.o
.PHONY : rein.cpp.o

rein.i: rein.cpp.i

.PHONY : rein.i

# target to preprocess a source file
rein.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/rein.cpp.i
.PHONY : rein.cpp.i

rein.s: rein.cpp.s

.PHONY : rein.s

# target to generate assembly for a file
rein.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/rein.cpp.s
.PHONY : rein.cpp.s

run_models.o: run_models.cpp.o

.PHONY : run_models.o

# target to build an object file
run_models.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/run_models.cpp.o
.PHONY : run_models.cpp.o

run_models.i: run_models.cpp.i

.PHONY : run_models.i

# target to preprocess a source file
run_models.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/run_models.cpp.i
.PHONY : run_models.cpp.i

run_models.s: run_models.cpp.s

.PHONY : run_models.s

# target to generate assembly for a file
run_models.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/run_models.cpp.s
.PHONY : run_models.cpp.s

test.o: test.cpp.o

.PHONY : test.o

# target to build an object file
test.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/test.cpp.o
.PHONY : test.cpp.o

test.i: test.cpp.i

.PHONY : test.i

# target to preprocess a source file
test.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/test.cpp.i
.PHONY : test.cpp.i

test.s: test.cpp.s

.PHONY : test.s

# target to generate assembly for a file
test.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/test.cpp.s
.PHONY : test.cpp.s

util.o: util.cpp.o

.PHONY : util.o

# target to build an object file
util.cpp.o:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/util.cpp.o
.PHONY : util.cpp.o

util.i: util.cpp.i

.PHONY : util.i

# target to preprocess a source file
util.cpp.i:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/util.cpp.i
.PHONY : util.cpp.i

util.s: util.cpp.s

.PHONY : util.s

# target to generate assembly for a file
util.cpp.s:
	$(MAKE) -f CMakeFiles/cmakeRB.dir/build.make CMakeFiles/cmakeRB.dir/util.cpp.s
.PHONY : util.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... cmakeRB"
	@echo "... edit_cache"
	@echo "... BIOP.o"
	@echo "... BIOP.i"
	@echo "... BIOP.s"
	@echo "... BIOP1.o"
	@echo "... BIOP1.i"
	@echo "... BIOP1.s"
	@echo "... BIOP2.o"
	@echo "... BIOP2.i"
	@echo "... BIOP2.s"
	@echo "... BIOP3.o"
	@echo "... BIOP3.i"
	@echo "... BIOP3.s"
	@echo "... BIOP4.o"
	@echo "... BIOP4.i"
	@echo "... BIOP4.s"
	@echo "... BIOP5.o"
	@echo "... BIOP5.i"
	@echo "... BIOP5.s"
	@echo "... constant.o"
	@echo "... constant.i"
	@echo "... constant.s"
	@echo "... generator.o"
	@echo "... generator.i"
	@echo "... generator.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... printer.o"
	@echo "... printer.i"
	@echo "... printer.s"
	@echo "... rein.o"
	@echo "... rein.i"
	@echo "... rein.s"
	@echo "... run_models.o"
	@echo "... run_models.i"
	@echo "... run_models.s"
	@echo "... test.o"
	@echo "... test.i"
	@echo "... test.s"
	@echo "... util.o"
	@echo "... util.i"
	@echo "... util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

