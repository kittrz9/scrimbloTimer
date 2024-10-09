#!/bin/sh

set -xe

BASEDIR="$(dirname $0 | xargs readlink -f)"
cd $BASEDIR

SDL_URL="https://github.com/libsdl-org/SDL/releases/download/preview-3.1.3/SDL3-3.1.3.tar.xz"
SDL_NAME="SDL3-3.1.3"

CC=clang
CFLAGS="-g -Wall -Wextra -Wpedantic"
INCLUDES="-I$SDL_NAME/include"
# I think I'm still probably not using rpath correctly lmao
LIBS="-Wl,-rpath=./ -Wl,-rpath=./build/ -L./build/ -lSDL3 -lm"

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
