cmake_minimum_required(VERSION 3.8.0)
project(banana_client VERSION 0.0.1 LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS filesystem)

find_package(banana REQUIRED CONFIG)

add_executable(banana_client main.cpp)
target_link_libraries(banana_client 
  PRIVATE
  banana_box 
  banana_peel
  )