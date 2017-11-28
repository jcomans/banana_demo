add_library(banana_box INTERFACE)

set_property(TARGET banana_box PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_11)

target_link_libraries(banana_box
  INTERFACE
  banana
  Boost::filesystem
  )

install(TARGETS banana_box EXPORT bananaConfig)
