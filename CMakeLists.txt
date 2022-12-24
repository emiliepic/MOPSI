cmake_minimum_required(VERSION 3.0)
project(MOPSI)
find_package(Imagine REQUIRED)


ImagineAddExecutable(mopsi main.cpp tree.h gradient.h shortest_path.h)
ImagineUseModules(mopsi Graphics)
