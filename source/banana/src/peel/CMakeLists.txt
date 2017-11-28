add_library(banana_peel STATIC peel.cpp)

set_property(TARGET banana_peel PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(banana_peel
  PUBLIC
  banana
  )

install(TARGETS banana_peel EXPORT bananaConfig
  ARCHIVE DESTINATION lib
  )