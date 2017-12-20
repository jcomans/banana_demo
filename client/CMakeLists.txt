cmake_minimum_required(VERSION 3.8.0)
project(banana_client VERSION 0.0.1 LANGUAGES CXX)

# The banana library needs boost, so we need to find it first
find_package(Boost REQUIRED COMPONENTS filesystem)

# Import our library targets (banana, banana_box, banana_peel) with
# find_package and the bananaConfig.cmake file created by CMake itself
find_package(Banana REQUIRED CONFIG)

add_executable(banana_client main.cpp)
target_link_libraries(banana_client 
  PRIVATE
  Banana::box 
  Banana::peel
  # On windows, force normal linking instead of auto-linking through the pseudo targets
  # Boost::disable_autolinking Boost::dynamic_linking
  Boost::disable_autolinking
  Boost::dynamic_linking
  )
