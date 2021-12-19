add_executable(ref ref.cpp)
target_link_libraries(ref PRIVATE xmd)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
