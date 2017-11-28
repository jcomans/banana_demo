# Modern CMake Demo: banana lib

The goal of this project is to demonstrate how a library can be implemented using modern CMake constructs.

## What is modern CMake

Modern Cmake referes to a style of writing CMake scripts that make heavy use of the target centric commands that have been available since CMake 3.0

### Legacy CMake

Legacy CMake (2.8 and before) relied heavily on global or directory scoped variables and commands to control how targets are built. For example, to build an executable with an additional include directory, the following commands were used:

```cmake
include_directories(/foo/bar/fubar)
add_executable(baz main.cpp)
```

This would add `-I/foo/bar/fubar` to the command line of the compiler when compiling `main.cpp.o`. However, `include_directories` adds its paths to the directory scope. This means that any target (an executable or library) defined in that CMakeLists.txt file, or any CMakeLists.txt in a subdirectory would also have this include directory added. In certain cases this can be handy (when multiple targets require the same include directory) but most of the time it is confusing since the actual configuration for a target depends on the full hiearchy of the project.

Another common theme in legacy CMake is manual dependency management. For example, if we would have an executable `aaa` that depends on our own library `bbb` which in turn needs `boost` we would need to manually add the proper include directories for `bbb` and `boost`. The same holds for compiler flags and definitions required for the dependencies. The only thing we got was that `aaa` would automatically link `boost` when linking with `bbb` when we link with `bbb`.

### Modern CMake

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

## Structure of the demo project

## Building the demo project