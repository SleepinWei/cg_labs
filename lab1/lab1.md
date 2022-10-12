# Lab1 Rasterization of the Circle

## Description

To facilitate checking the correctness of what you have done, we provide a simple version of the result.(res.jpg)

The rasterization algorithm must contain bresenham as well as dda and performance evaluation between them is supposed to do.

## Framework

We provide the environment configuration on the Ubuntu20.0x Platform

As for windows users, please help yourself.

* config.sh
    the scripts to install the opengl and glm on your Ubuntu, which must be executed under the root priviledge.

* compile_project.sh
    the scripts to compile the testing project -- drawing the points.

* include/point.hpp
    encapsulate the point structure.

* include/algorithm.hpp
    encapsulate the algorithm class. You are supposed to fill your code in this file.

* include/buffer_screen.hpp
    encapsulate the buffer class and screen class.
    
* main.cpp
    provide the mainloop for rendering.

After running compile_project.sh, you can find an executable named `circle` in `build` directory.

\* .sh files are bash scripts, thus they only works on linux.

## Instruction

In this lab, you should

* implement two functions in `include/algorithm.cpp`. The places where you should fill your code in are marked with `\\TODO:`
  * In class `DDA`, you should implement DDA algorithm to draw a circle.
  * In class `Bresham`, you should implement bresenham algorithm to draw a circle.

* compare the performance of two algorithms and explain why the gap of performance exists.

## Submission

You should submit your **code** along with **a report**.

Please submit on canvas before deadline.

Pack your program so that it can run on **BARE computers**, which means you should provide glfw \& glad & glm environment and .sln or CMakeLists files.

In this lab, linux users may not bother to provide the environment. But this **should be a must** for windows users.

Providing an executable is strongly recommended.

**Plagiarism will not be tolerated.**