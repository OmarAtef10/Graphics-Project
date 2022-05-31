cmake_minimum_required(VERSION 3.19)
project(Final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Final
        Circle.cpp
        Circle.h
        Clippings.h
        Curve.cpp
        Curve.h
        Elipse.h
        Fillings.cpp
        Fillings.h
        Final.cpp
        Final.h
        framework.h
        GraphicsProject.cpp
        GraphicsProject.h
        LibRes.h
        Line.cpp
        Line.h
        Resource.h
        targetver.h
        UJtils.h)
