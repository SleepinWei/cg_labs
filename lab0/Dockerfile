FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
USER root
RUN mv /etc/apt/sources.list /etc/apt/sources_backup.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal main restricted " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-updates main restricted " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal universe " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-updates universe " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal multiverse " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-updates multiverse " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-backports main restricted universe multiverse " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-security main restricted " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-security universe " >> /etc/apt/sources.list && \
echo "deb http://mirrors.ustc.edu.cn/ubuntu/ focal-security multiverse " >> /etc/apt/sources.list && \
echo "deb http://archive.canonical.com/ubuntu focal partner " >> /etc/apt/sources.list && \ 
cat /etc/apt/sources.list

RUN apt-get update -y
RUN apt-get install -y build-essential \ 
    libgl1-mesa-dev \ 
    mesa-utils \ 
    libglm-dev \ 
    libglfw3-dev \ 
    cmake 