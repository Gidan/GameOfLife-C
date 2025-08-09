#!/bin/zsh

set -xe
gcc -o build/main src/main.c -lraylib -I/opt/homebrew/Cellar/raylib/5.5/include -L /opt/homebrew/Cellar/raylib/5.5/lib
