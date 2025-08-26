#/bin/bash


rm -rf ./deps/**
mkdir ./working

sh ./bash/compile_sdl3.sh
sh ./bash/compile_sdl3_image.sh

rm -rf ./working