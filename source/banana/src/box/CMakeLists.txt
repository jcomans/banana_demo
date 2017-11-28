# banana_box is also an interface library. Nothing gets build, but there are some
# requirements on the client. They need boost and they need to compile using
# c++11
add_library(banana_box INTERFACE)

# Indicate that c++11 is required. We would prefer to use target_compile_features
# but that command is not available for INTERFACE libraries. Instead, we directly
# set the INTERFACE_COMPILE_FEATURES property to cxx_std_11. 
set_property(TARGET banana_box PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_11)

# We specify that clients linking to banana_box will need to link to banana and
# Boost::filesystem. This will configure the include paths, compiler flags, 
# linker flags, etc in the right way.
target_link_libraries(banana_box
  INTERFACE
  banana
  Boost::filesystem
  )

# Again, no files ar installed, but the target and its properties are added
# to the bananaConfig file.
install(TARGETS banana_box EXPORT bananaConfig)
