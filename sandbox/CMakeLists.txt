add_executable(reference reference.cpp)
target_link_libraries(reference PRIVATE xmd)

add_executable(taskflow taskflow.cpp)
target_link_libraries(taskflow PRIVATE xmd)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
