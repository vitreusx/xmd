add_executable(xmd main.cpp)

add_subdirectory(base)
add_subdirectory(cpu_ref)

target_link_libraries(xmd
    PRIVATE base cpu_ref)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
    TARGET xmd
    POST_BUILD
    COMMAND objdump -drlwC -Mintel xmd >xmd.s
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})