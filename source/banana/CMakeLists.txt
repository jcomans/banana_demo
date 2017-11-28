project(banana)

# We'll define some options to allow selective builds of the
# implementation libraries
option(ENABLE_BANANA_BOX  "Enable the banana box library"  ON)
option(ENABLE_BANANA_PEEL "Enable the banana peel library" ON)

# The first component of the library will be an interface target.
# Interface targets don't build libraries or executables, they
# are pseudo targets. In this case, it is meant to represent the
# header-only part of our library. Clients that only need the 
# headers will be able to simply add 'banana' to target_link_libraries.
# That will make sure the proper include paths are added
add_library(banana INTERFACE)

# Here we specify where the header files for our banana lib are
# located. There's a couple of new things in here. The 'INTERFACE'
# keyword indicates that any targets linking to 'banana' will need
# to add the following directories to its include directories.
# The directories themselves use 'generator' expressions. These
# are used to figure out the proper directories during 'export'
# which will come up later
target_include_directories(banana
  INTERFACE
  # The include directory wrt our build dir
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  # The include directory wrt our install environment
  $<INSTALL_INTERFACE:include>
  )

# Installing the 'banana' target does nothing, except add it to
# our bananaConfig export. Which will be discussed below
install(TARGETS banana EXPORT bananaConfig)
# This actually installs the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/banana
  DESTINATION include
  )

if(ENABLE_BANANA_BOX)
  add_subdirectory(src/box)
endif()

if(ENABLE_BANANA_PEEL)
  add_subdirectory(src/peel)
endif()

# The install(EXPORT command will generate and install a
# bananaConfig.cmake file. This file can then be used in
# a different CMake project by using find_package(banana).
# As a result of this find package, all targets that have been
# installed with the bananaConfig export (banana, banana_box, bana
install(EXPORT bananaConfig DESTINATION share/banana/cmake)
export(EXPORT bananaConfig)