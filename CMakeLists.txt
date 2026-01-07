cmake_minimum_required(VERSION 3.16)
project(Plotter)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        Canvas.hpp
        CanvasIterators.hpp
        Canvas.cpp
        Plotter.cpp
        Plotter.hpp
        GrayscalePlotter.cpp
        GrayscalePlotter.hpp
        Config.cpp
        Canvas.hpp
        DemoRunner.cpp
        DemoRunner.hpp
        json.cpp
        json.h
        main.cpp
)

add_executable(Plotter ${SOURCES})
