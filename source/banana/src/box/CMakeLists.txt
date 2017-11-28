add_library(banana_box INTERFACE)

target_link_libraries(banana_box
  INTERFACE
  banana
  Boost::filesystem
  )

install(TARGETS banana_box EXPORT bananaConfig)