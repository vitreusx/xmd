add_executable(xmd main.cpp)

add_subdirectory(base)
add_subdirectory(cpu_ref)

target_link_libraries(xmd
    PRIVATE base cpu_ref)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")