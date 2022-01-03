add_executable(ref ref.cpp)
target_link_libraries(ref PRIVATE xmd)

target_compile_options(ref
    PRIVATE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-g>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-g;-O2;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(test test.cpp)
target_link_libraries(test PRIVATE xmd)

target_compile_options(test
    PRIVATE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-g>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-g;-O2;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

add_custom_command(
    TARGET test
    POST_BUILD
    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})