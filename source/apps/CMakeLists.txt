add_executable(tree_app tree_app.cpp)
target_link_libraries(tree_app PRIVATE banana)

if(TARGET banana_box)
  add_executable(box_app box_app.cpp)
  target_link_libraries(box_app PRIVATE banana_box)
endif()

if(TARGET banana_peel)
  add_executable(peel_app peel_app.cpp)
  target_link_libraries(peel_app PRIVATE banana_peel)
endif()