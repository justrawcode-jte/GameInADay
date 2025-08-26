#/bin/bash

url="https://github.com/libsdl-org/SDL_image.git"

git clone $url ./working/sdl3_image

cd ./working/sdl3_image

cmake -S ./ -B ./build -DSDL3_DIR=../../deps/sdl3/lib/cmake/SDL3
cmake --build ./build
cmake --install ./build --prefix ../../deps/sdl3_image

cd ../../