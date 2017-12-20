# Now we'll create a proper library. This one will be static for simplicity
# and only uses one source file
add_library(banana_peel STATIC peel.cpp)

# Create a namespaced alias
add_library(Banana::peel ALIAS banana_peel)

# When exporting the banana_peel target, the Banana:: namespace is added. This
# would result in a Banana::banana_peel targt, which we don't want. We will adjust
# the exported name to drop the banana_ part so that the end result will be
# Banana::peel
set_property(TARGET banana_peel PROPERTY EXPORT_NAME peel)

# The library might be embeded in a shared object or dll later. On some
# platforms (e.g. Linux) this requires building position indepenent code.
# Instead of having a bunch of platform dependent code, we set the
# POSITION_INDEPENDENT_CODE property to ON. This will do the right thing
# depending on the platform
set_property(TARGET banana_peel PROPERTY POSITION_INDEPENDENT_CODE ON)

# We need to compile this library using c++11. This also implies clients
# need to do the same thing. The PUBLIC tag ensures both our build as well
# as the clients build will use c++11.
target_compile_features(banana_peel PUBLIC cxx_std_11)

# Linking our main banana target will ensure we have the proper include
# directories in place. Again we use PUBLIC because both the library and
# the client code will need to include the header directory
target_link_libraries(banana_peel
  PUBLIC
  Banana::banana
  )

# This time, we will be installing a file, so we need to specify a
# destination next to our export definition
install(TARGETS banana_peel EXPORT BananaConfig
  ARCHIVE DESTINATION lib
  )
