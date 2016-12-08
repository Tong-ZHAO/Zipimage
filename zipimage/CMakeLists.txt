project (ZipImage)

cmake_minimum_required(VERSION 2.6)

file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}" d)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${d}/CMake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Imagine)

#ImagineAddExecutable(test_zipimage example.cpp ../quadtree/quadtree.h)
ImagineAddExecutable(zipimage main.cpp ../quadtree/quadtree.h FileCompress.h FileCompress.cpp TreeCompress.h TreeCompress.cpp)

#ImagineUseModules(test_zipimage Graphics)
ImagineUseModules(zipimage Graphics)
