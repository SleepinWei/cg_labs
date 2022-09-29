#!/bin/bash

# glfw ready work
apt-get install -y libxss-dev libxxf86vm-dev libxkbfile-dev libxv-dev libx11-dev
apt-get install -y libxrandr-dev libxi-dev libxinerama-dev libsdl2-dev libxcursor-dev

# git clone glfw and compile 
if [ ! -d "glfw" ]; then
    git clone https://github.com/glfw/glfw.git
fi
if [ -d "glfw/build" ]; then
    rm -rf glfw/build
fi
cd glfw && mkdir build && cd build && cmake .. && make -j8 && make install && cd ../../
#rm -rf glfw

# git clone glm and compile
if [ ! -d "glm" ]; then
    git clone https://github.com/g-truc/glm.git
fi
if [ -d "glm/build" ]; then
    rm -rf glm/build
fi
cd glm && mkdir build && cd build && cmake .. && make -j8 && make install && cd  ../../
#rm -rf glm

./compile_project.sh
