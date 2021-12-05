#add_executable(xmd main.cpp)
#
#add_subdirectory(base)
#add_subdirectory(ref)

#target_link_libraries(xmd
#    PRIVATE base ref)
#
#target_compile_options(xmd
#    PRIVATE
#    $<$<CONFIG:Debug>:-g;$<$<COMPILE_LANGUAGE:CXX>:-Og>>
#    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
#    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)
#
#file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
#
#add_custom_command(
#    TARGET xmd
#    POST_BUILD
#    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#
#add_executable(experiments experiments.cpp)
#
#target_link_libraries(experiments
#    PRIVATE base ref)
#
#target_include_directories(experiments
#    PRIVATE ext)
#
#target_compile_options(experiments
#    PRIVATE
#    $<$<CONFIG:Debug>:-g;$<$<COMPILE_LANGUAGE:CXX>:-Og>>
#    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
#    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)
#
#add_custom_command(
#    TARGET experiments
#    POST_BUILD
#    COMMAND objdump -drlwC -Mintel experiments >experiments.s
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(ideal)