#/bin/bash

url="https://github.com/libsdl-org/SDL.git"

git clone $url ./working/sdl3

cd ./working/sdl3

cmake -S ./ -B ./build
cmake --build ./build
cmake --install ./build --prefix ../../deps/sdl3

cd ../../