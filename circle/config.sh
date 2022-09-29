#!/bin/bash

# git clone glfw and compile 
git clone https://github.com/glfw/glfw.git
cd glfw && mkdir build && cd build && cmake .. && make -j8 && make install
rm -rf glfw

# git clone glm and compile
git clone https://github.com/g-truc/glm.git
cd glm && mkdir build && cd build && cmake .. && make -j8 && make install
rm -rf glm

chmod +x ./compile.sh && ./compile_project.sh

