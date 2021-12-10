add_executable(sandbox main.cpp)
target_link_libraries(sandbox PRIVATE xmd)
file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
