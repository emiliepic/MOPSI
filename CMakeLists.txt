cmake_minimum_required(VERSION 3.0)
project(ZipImage)
find_package(Imagine REQUIRED)

add_subdirectory(quadtree)

ImagineAddExecutable(example example.cpp quadtree/quadtree.h)
ImagineUseModules(example Graphics)
