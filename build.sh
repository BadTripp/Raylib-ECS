#!/bin/bash

cc ecs.c -o ecs \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lraylib \
  -framework OpenGL \
  -framework Cocoa \
  -framework IOKit \
  -framework CoreVideo

./ecs

