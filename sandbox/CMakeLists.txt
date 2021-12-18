add_executable(ref ref.cpp)
target_link_libraries(ref PRIVATE xmd)

add_executable(omp omp.cpp)
target_link_libraries(omp PRIVATE xmd)

target_compile_options(omp
    PRIVATE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-Og>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-g;-Wall;-Wextra;-Wpedantic>)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
    TARGET omp
    POST_BUILD
    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})