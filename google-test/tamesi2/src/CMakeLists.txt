cmake_minimum_required(VERSION 3.10)

file(GLOB SOURCE *.cc *.cpp)
add_library(torima ${SOURCE})
