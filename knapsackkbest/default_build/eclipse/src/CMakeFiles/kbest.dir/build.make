# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse"

# Include any dependencies generated for this target.
include src/CMakeFiles/kbest.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/kbest.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/kbest.dir/flags.make

src/CMakeFiles/kbest.dir/kbest.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/kbest.c.o: ../../src/kbest.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/kbest.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/kbest.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest.c"

src/CMakeFiles/kbest.dir/kbest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/kbest.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest.c" > CMakeFiles/kbest.dir/kbest.c.i

src/CMakeFiles/kbest.dir/kbest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/kbest.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest.c" -o CMakeFiles/kbest.dir/kbest.c.s

src/CMakeFiles/kbest.dir/kbest.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/kbest.c.o.requires

src/CMakeFiles/kbest.dir/kbest.c.o.provides: src/CMakeFiles/kbest.dir/kbest.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/kbest.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/kbest.c.o.provides

src/CMakeFiles/kbest.dir/kbest.c.o.provides.build: src/CMakeFiles/kbest.dir/kbest.c.o

src/CMakeFiles/kbest.dir/kbest_io_utility.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/kbest_io_utility.c.o: ../../src/kbest_io_utility.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/kbest_io_utility.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/kbest_io_utility.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest_io_utility.c"

src/CMakeFiles/kbest.dir/kbest_io_utility.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/kbest_io_utility.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest_io_utility.c" > CMakeFiles/kbest.dir/kbest_io_utility.c.i

src/CMakeFiles/kbest.dir/kbest_io_utility.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/kbest_io_utility.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kbest_io_utility.c" -o CMakeFiles/kbest.dir/kbest_io_utility.c.s

src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.requires

src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.provides: src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.provides

src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.provides.build: src/CMakeFiles/kbest.dir/kbest_io_utility.c.o

src/CMakeFiles/kbest.dir/utility/utility.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/utility/utility.c.o: ../../src/utility/utility.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/utility/utility.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/utility/utility.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/utility/utility.c"

src/CMakeFiles/kbest.dir/utility/utility.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/utility/utility.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/utility/utility.c" > CMakeFiles/kbest.dir/utility/utility.c.i

src/CMakeFiles/kbest.dir/utility/utility.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/utility/utility.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/utility/utility.c" -o CMakeFiles/kbest.dir/utility/utility.c.s

src/CMakeFiles/kbest.dir/utility/utility.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/utility/utility.c.o.requires

src/CMakeFiles/kbest.dir/utility/utility.c.o.provides: src/CMakeFiles/kbest.dir/utility/utility.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/utility/utility.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/utility/utility.c.o.provides

src/CMakeFiles/kbest.dir/utility/utility.c.o.provides.build: src/CMakeFiles/kbest.dir/utility/utility.c.o

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o: ../../src/kp_algorithm/kp_alg_forward.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_forward.c"

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_forward.c" > CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.i

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_forward.c" -o CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.s

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.requires

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.provides: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.provides

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.provides.build: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o: ../../src/kp_algorithm/kp_alg_backtrack.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_backtrack.c"

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_backtrack.c" > CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.i

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_algorithm/kp_alg_backtrack.c" -o CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.s

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.requires

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.provides: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.provides

src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.provides.build: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o: src/CMakeFiles/kbest.dir/flags.make
src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o: ../../src/kp_model/kp_inner_model.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/CMakeFiles" $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o   -c "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_model/kp_inner_model.c"

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.i"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_model/kp_inner_model.c" > CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.i

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.s"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src/kp_model/kp_inner_model.c" -o CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.s

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.requires:
.PHONY : src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.requires

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.provides: src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.requires
	$(MAKE) -f src/CMakeFiles/kbest.dir/build.make src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.provides.build
.PHONY : src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.provides

src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.provides.build: src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o

# Object files for target kbest
kbest_OBJECTS = \
"CMakeFiles/kbest.dir/kbest.c.o" \
"CMakeFiles/kbest.dir/kbest_io_utility.c.o" \
"CMakeFiles/kbest.dir/utility/utility.c.o" \
"CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o" \
"CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o" \
"CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o"

# External object files for target kbest
kbest_EXTERNAL_OBJECTS =

src/libkbest.so: src/CMakeFiles/kbest.dir/kbest.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/kbest_io_utility.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/utility/utility.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o
src/libkbest.so: src/CMakeFiles/kbest.dir/build.make
src/libkbest.so: src/CMakeFiles/kbest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libkbest.so"
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kbest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/kbest.dir/build: src/libkbest.so
.PHONY : src/CMakeFiles/kbest.dir/build

src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/kbest.c.o.requires
src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/kbest_io_utility.c.o.requires
src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/utility/utility.c.o.requires
src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_forward.c.o.requires
src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/kp_algorithm/kp_alg_backtrack.c.o.requires
src/CMakeFiles/kbest.dir/requires: src/CMakeFiles/kbest.dir/kp_model/kp_inner_model.c.o.requires
.PHONY : src/CMakeFiles/kbest.dir/requires

src/CMakeFiles/kbest.dir/clean:
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" && $(CMAKE_COMMAND) -P CMakeFiles/kbest.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/kbest.dir/clean

src/CMakeFiles/kbest.dir/depend:
	cd "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest" "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/src" "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse" "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src" "/home/daniele/Dropbox/University/Metodi e Modelli per il Supporto alle Decisioni LM/Project/knapsackkbest/default_build/eclipse/src/CMakeFiles/kbest.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/kbest.dir/depend

