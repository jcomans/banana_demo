# Modern CMake Demo: banana lib

The goal of this project is to demonstrate how a library can be implemented using modern CMake constructs.

## What is modern CMake

Modern Cmake referes to a style of writing CMake scripts that make heavy use of the target centric commands that have been available since CMake 3.0

Even more info can be found on this [reddit topic](https://www.reddit.com/r/cpp/comments/6fm38k/idiomatic_cmake/?st=j3mnsy0n&sh=13d47a4c).

### Legacy CMake

Legacy CMake (2.8 and before) relied heavily on global or directory scoped variables and commands to control how targets are built. For example, to build an executable with an additional include directory, the following commands were used:

```cmake
include_directories(/foo/bar/fubar)
add_executable(baz main.cpp)
```

This would add `-I/foo/bar/fubar` to the command line of the compiler when compiling `main.cpp.o`. However, `include_directories` adds its paths to the directory scope. This means that any target (an executable or library) defined in that CMakeLists.txt file, or any CMakeLists.txt in a subdirectory would also have this include directory added. In certain cases this can be handy (when multiple targets require the same include directory) but most of the time it is confusing since the actual configuration for a target depends on the full hiearchy of the project.

Another common theme in legacy CMake is manual dependency management. For example, if we would have an executable `aaa` that depends on our own library `bbb` which in turn needs `boost` we would need to manually add the proper include directories for `bbb` and `boost`. The same holds for compiler flags and definitions required for the dependencies. The only thing we got was that `aaa` would automatically link `boost` when linking with `bbb` when we link with `bbb`.

### Modern CMake

#### Target Centric Approach

The killer feature for modern CMake is that it allows a fully target centric approach. This means that we start by defining the sources from which a target is built (through `add_executable or add_library`) and then specify for that target what include directories, compiler flags, and linker flags are required. These target properties come in two flavors: properties and flags required to actually build the target, and properties and flags required by other targets that want to use the current target.

As an example, assume we are building a library that uses boost headers in its implementation, but not in its interface. In this case we would use:

```cmake
target_include_directories(mylib PRIVATE /foo/bar/boost)
```

This will add `-I/foo/bar/boost` to the command line when building `mylib` but it won't affect targets that link to `mylib`.

If our library would use boost in its interface, other targets using `mylib` would also need to add the include directory to their command line. In order to do this, we would use:

```cmake
target_include_directories(mylib PUBLIC /foo/bar/boost)
```

The PUBLIC keyword ensures that `-I/foo/bar/boost` is used when building `mylib` but it will also propagate to other targets. If we would create `myexe`, we can simply use:

```cmake
target_link_libraries(myexe mylib)
```

This will not only link `myexe` to `mylib`, but it will also automatically add `-I/foo/bar/boost` when compiling `myexe`.

#### Exporting Configuration

In legacy CMake, whenever we would use an external library, we would use the find_package command which would locate the library and set a number of (global) variables that can be used to configure includes and linking. Typically a package would define variables like `mylib_FOUND`, `mylib_INCLUDE_DIRS`, and `mylib_LIBRARIES`. Then for our own targets we would use these to manually configure the build using these variables.

With the target centric approach, however, during the build of the library, CMake knows everything needed to incorporate the target being generated (from the PUBLIC and INTERFACE properties). Using this information, CMake is able to export a configuration file containing this information. This config file will contain all exported targets, location of the actual libraries, and properties required for linking to them. Using this config file, `find_package` is able to parse this file. After this, instead of having global variables, there will be _imported_ targets (targets that don't build anything, but that can be linked to to pull in include and link configuration).

The configuration file is configured using the `install(TARGET` commands, and is physically installed during the `make install` phase. This installed configuration file relies on relative paths wrt to the installation directory, so the result of this `make install` can be distributed as normal. Sometimes, during development, it can be handy to be able to use the library in an external project while developing it at the same time. For this, CMake is also able to generate a config file in the build directory (as opposed to the install directory).

#### Creating Targets for Legacy find_package Modules

CMake's own `find_package` scripts are being converted to also define imported targets. An example of this is the Boost `find_package` module. In the later versions of CMake next to the traditional Boost_INCLUDE_DIRECTORIES etc., it also defines imported targets like `Boost::boost`, `Boost::filesystem`, `Boost::random`, etc. that can be used directly in `target_link_libraries`.

For find scripts that don't offer this yet, it is relatively simple to do this manually:
```cmake
find_package(openssl REQUIRED)

add_library(openssl INTERFACE IMPORTED)
set_property(TARGET openssl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
set_property(TARGET openssl PROPERTY INTERFACE_LINK_LIBRARIES ${OPENSSL_LIBRARIES})
```
After this, all we need to do to use openssl is use `target_link_libraries(mytarget PUBLIC openssl)`.

## Structure of the demo project

The demo project in this repository consists of a dummy library `banana` and a simple external client program that uses the library. The library is in the `source` directory, the client program in the `client` directory.

The `banana` library itself is structured in a `boost` like fashion. Some parts are header only (`box.hpp` and `peel.hpp`) some components need an implementation library (`tree.hpp`). The `box` component is header only, but has a dependency on Boost.filesystem.

This results in three targets being defined:
1. banana: used to pull in the include directory. Clients only using the header only part can link to the banana target to make sure their include paths are properly configured.

2. banana_box: Clients using the `box.hpp` header also need boost. The `banana_box` target ensures that the correct libraries from boost are linked in. It is however important to notice that it is the responsibility of the client to actually make sure `boost` is found and properly configured before use. In tis case this means that the targets `Boost::boost` and `Boost::filesystem` should exist.

3. banana_peel: This is an actual library. No special features here.

One of the goals of the new Cmake approach is to have as little dependencies as possible. Therefore, an ideal library setup knows nothing of its surroundings, except for some target names. In this way, libraries can be extracted and moved to different projects. Or made into standalone projects.

## Building the demo project

### banana library
Start by crating a `build` directory next to the `source` and `client` directory. 

On Linux, use the following command from inside the build directory (setting BOOST_ROOT is only necessary if boost is not in a default location):

```cmake
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=<boost_dir> ../source/
```

On Windows, open the CMake GUI, and selct `source` and `build` as the source and build directory. Before clicking `Configure` and `Generate`, add the following variables to the cache:
```
CMAKE_INSTALL_PREFIX: ../install
BOOST_ROOT: <boost_dir>
```

Build and install the project using your prefered method:
1. Building the `INSTALL` project in VisualStudio
2. Using `make install` on the Linux command line
3. Using `cmake --build . --target install` on a Linux or Windows command line

### client executable
Create a `client_build` directory next to the `client` directory.

Configure CMake again as before (you can skipp the install however), but add one extra cache variable `banana_DIR` either on the command line with `-D` or in the GUI. This variable will be used by find_package to locate the bananaConfig.cmake file and import the proper targets. You have two options here. You can either use `../install/share/banana/cmake` to use the installed version of the library. Or you can use `../build/banana` to use the _development_ version of the library straight out of the build directory.

You can build with `make VERBOSE=y` or inspect the configuration in VisualStudio to verify that all the correct dependencies in terms of include directories and link libraries have been configured correctly.