# Most modern cmake constructs have been available since 3.0, but
# lets pick a fairly recent one here
cmake_minimum_required(VERSION 3.8.0)
# We'll only use c++ here
project(banana_demo VERSION 0.0.1 LANGUAGES CXX)

# Modern boost find scripts define proper imported targets. To use the find
# script to locate our custom boost, we would need to set BOOST_ROOT
find_package(Boost REQUIRED COMPONENTS filesystem)
# The result of this package will allow us to use Boost::boost and
# Boost::filesystem in 'target_link_libraries'. This will automatically
# add the proper include directories and link libraries.
# In tis case, this will also automatically add Boost::system which is
# required for Boost::filesystem

add_subdirectory(banana)
add_subdirectory(apps)