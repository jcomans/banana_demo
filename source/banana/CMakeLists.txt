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

# We will create an alias for this target that is namespaced. This allows
# us to namespace targets. Next to grouping targets logically this has
# another advantage. When using target names with ::, CMake will
# complain during generation if the target doesn't exist (in case
# of a type for instance). With a regular target name, CMake asumes
# you are providing the name of a link library and will hapily continue.
# You first indication that somthing is wrong would then be when
# linking fails.
add_library(Banana::banana ALIAS banana)

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
install(TARGETS banana EXPORT BananaConfig)
# This actually installs the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/banana
  DESTINATION include
  )

# The banana box library is also a header only library, but with a
# dependency on boost
if(ENABLE_BANANA_BOX)
  add_subdirectory(src/box)
endif()

# The banana peel library has an actual implementation library for
# its implementaion
if(ENABLE_BANANA_PEEL)
  add_subdirectory(src/peel)
endif()

# The install(EXPORT command will generate and install a
# bananaConfig.cmake file. This file can then be used in
# a different CMake project by using find_package(Banana).
install(EXPORT BananaConfig DESTINATION share/banana/cmake NAMESPACE Banana::)
# The export command wil generate a BananaConfig.cmake file in the
# current build directory. With this, external projects can also
# use the library from the build directory using the same find_package
# command as above. This is mainly usefull in development setups.
export(EXPORT BananaConfig NAMESPACE Banana::)
