#!/bin/sh

set -xe

BASEDIR="$(dirname $0 | xargs readlink -f)"
cd $BASEDIR

CC=clang
REQUIRED_COMMANDS="$CC cmake wget tar"

for cmd in $REQUIRED_COMMANDS; do
	if [ -z "$(command -v $cmd)" ]; then
		echo "\n\nERROR: could not find command \"$cmd\", which is required to build this\n\n"
		exit 1
	fi
done

SDL_URL="https://github.com/libsdl-org/SDL/releases/download/preview-3.1.3/SDL3-3.1.3.tar.xz"
SDL_NAME="SDL3-3.1.3"

CFLAGS="-g -Wall -Wextra -Wpedantic"
INCLUDES="-I$SDL_NAME/include"
# I think I'm still probably not using rpath correctly lmao
LIBS="-Wl,-rpath=./ -Wl,-rpath=./build/ -L./build/ -lSDL3 -lm -lX11"

CFILES="$(find src/ -name "*.c")"

if [ ! -f "./$SDL_NAME.tar.xz" ]; then
	wget "$SDL_URL" -O "$SDL_NAME.tar.xz"
fi

if [ ! -d "./$SDL_NAME" ]; then
	tar -xavf "$SDL_NAME.tar.xz"
fi

if [ ! -f "./$SDL_NAME/build/libSDL3.so" ]; then
	cd "$SDL_NAME"
	cmake -S . -B build
	cmake --build build -j "$(nproc)"
	cd "$BASEDIR"
fi

rm -rf build/ obj/
mkdir build/ obj/

cp "./$SDL_NAME/build/libSDL3.so" "./$SDL_NAME/build/libSDL3.so.0" build/

for f in $CFILES; do
	OBJNAME="$(echo $f | sed -e "s/src/obj/;s/\.c/\.o/")"
	$CC $INCLUDES $CFLAGS -c $f -o $OBJNAME
	OBJS="$OBJS $OBJNAME"
done

$CC $CFLAGS $OBJS $LIBS -o build/timer
