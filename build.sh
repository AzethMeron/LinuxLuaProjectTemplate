#/!bin/bash

##########################################################################################

# by Jakub Grzana

# Simple Bash script to make "projects" without any programming environment
# Dedicated for building my Lua-connected projects. You can remove Lua by commenting LUA_INC and LUA_LIB

# There are 3 directories used by "project":
# DIR_SRC, src/ by default - Directory for source files. All .cpp files are compilated, all the others - ignored
# DIR_INC, inc/ by default - Directory for header files. You can include those in .cpp files without caring for paths
# DIR_OBJ, obj/ by default - Directory for compiled source (.o) It's cleansed every time script is called

# You can pass additional arguments:
# [-p] - makes all compilation parallel (run in background) greatly improving performance
# [-c <string>] - additional arguments for compiler
# [-b <string>] - additional arguments for linker
# [-f <string>] - set name of executable after building

##########################################################################################

# Parameters
DIR_OBJ='.obj'
DIR_SRC='src'
DIR_INC='inc'
EXECUTABLE_NAME='main'
PARALLEL=0
COMPILER="g++" # for easy changing g++ to gcc
# Lua - you can remove lua from project by commenting those two
#LUA_INC='-I/usr/include/lua5.1' # flags for compilation
#LUA_LIB='-llua5.1' # flags for building
LUA_INC='-Iliblua/include' # flags for compilation
LUA_LIB='liblua/liblua5.1.a -ldl' # flags for building
# Flags
COMPILATION_FLAGS='-O2 -Wall -pedantic' # flags for compilation
BUILD_FLAGS='-lm -static' # flags for building

# Parsing arguments
while getopts "pc:b:f:?" opt; do
  case ${opt} in
    p ) # process option p
	PARALLEL=1
      ;;
	c ) # process option c
	COMPILATION_FLAGS="$COMPILATION_FLAGS $OPTARG"
      ;;
	b ) # process option c
        BUILD_FLAGS="$BUILD_FLAGS $OPTARG"
      ;;
	f ) # process option f
        EXECUTABLE_NAME="$OPTARG"
      ;;
    \? ) echo "Usage: ./build [-p] [-c <string>] [-b <string>] [-f <string>]"
	echo "-p = all compilations are done in background (parallel), compilation feedback might be mangled"
	echo "-c <string> = additional flags for compilation"
	echo "-b <string> = additional flags for building (linking)"
	echo "-f <string> = set name of executable"
	exit
      ;;
  esac
done

# Creating directories
mkdir -p $DIR_OBJ
mkdir -p $DIR_SRC
mkdir -p $DIR_INC

# Cleaning
rm -f $EXECUTABLE_NAME
rm -f $DIR_OBJ/*.o

# Compilation
echo 'Compiling...'
for i in $(ls $DIR_SRC/*.cpp) 
do
	# Make output (.o) filename - swapping src/ to obj/
	output_file=${i/$DIR_SRC/$DIR_OBJ}
	# Make output (.o) filename - swapping .cpp to .o
	output_file=${output_file/"cpp"/"o"}
	# Compilation itself
	compilation_cmd="$COMPILER $LUA_INC -I$DIR_INC -c $i $COMPILATION_FLAGS $CXXFLAGS -o $output_file"
	echo $compilation_cmd
	if [ $PARALLEL -eq 1 ]
	then
		$compilation_cmd &
	else
		$compilation_cmd
	fi
done

wait

# Linking
all_objects=$(ls $DIR_OBJ/*.o)
echo 'Linking...'
linking_cmd="$COMPILER $BUILD_FLAGS -o $EXECUTABLE_NAME $all_objects $LUA_LIB"
echo $linking_cmd
$linking_cmd
