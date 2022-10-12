
BUILD_PATH="./build"

if [ ! -d "$BUILD_PATH" ]; then
    mkdir $BUILD_PATH
fi

cd $BUILD_PATH && cmake .. && make -j8

if [ ! -d shader ]; then
    mkdir shader
fi
