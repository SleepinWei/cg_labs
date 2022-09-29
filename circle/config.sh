#!/bin/bash

# glfw ready work
apt-get install -y libxss-dev libxxf86vm-dev libxkbfile-dev libxv-dev libx11-dev
apt-get install -y libxrandr-dev libxi-dev libxinerama-dev libsdl2-dev libxcursor-dev

# git clone glfw and compile 
git clone https://github.com/glfw/glfw.git
cd glfw && mkdir build && cd build && cmake .. && make -j8 && make install
rm -rf glfw

# git clone glm and compile
git clone https://github.com/g-truc/glm.git
cd glm && mkdir build && cd build && cmake .. && make -j8 && make install
rm -rf glm

chmod +x ./compile_project.sh && ./compile_project.sh


