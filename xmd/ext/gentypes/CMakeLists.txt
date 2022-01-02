cmake_minimum_required(VERSION 3.20)
project(gentypes)

add_library(gentypes INTERFACE)

target_include_directories(gentypes
    INTERFACE include)

target_compile_features(gentypes
    INTERFACE cxx_std_17)