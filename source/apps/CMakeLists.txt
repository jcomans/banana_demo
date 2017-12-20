add_executable(tree_app tree_app.cpp)
target_link_libraries(tree_app PRIVATE Banana::banana)

if(TARGET banana_box)
  add_executable(box_app box_app.cpp)
  # On windows, force normal linking instead of auto-linking through the pseudo targets
  # Boost::disable_autolinking Boost::dynamic_linking
  target_link_libraries(box_app PRIVATE Banana::box Boost::disable_autolinking Boost::dynamic_linking)
endif()

if(TARGET banana_peel)
  add_executable(peel_app peel_app.cpp)
  target_link_libraries(peel_app PRIVATE Banana::peel)
endif()
