file(GLOB_RECURSE PRIVATE_SOURCES CONFIGURE_DEPENDS
    src/*.cpp src/*.cxx
    src/*.hpp src/*.h src/*.inl)

file(GLOB_RECURSE PUBLIC_SOURCES CONFIGURE_DEPENDS
    include/*.hpp include/*.h include/*.inl)

set(TARGET_NAME "xmd")
set(TARGET_ROOT_DIR "xmd")

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE ${PRIVATE_SOURCES}
    PUBLIC ${PUBLIC_SOURCES})

target_compile_features(${TARGET_NAME}
    PUBLIC cxx_std_17)

target_include_directories(${TARGET_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_ROOT_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/${TARGET_ROOT_DIR}>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_options(${TARGET_NAME}
    PRIVATE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-Og>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-g;-Wall;-Wextra;-Wpedantic>)

find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)
add_subdirectory(ext/cparse)

set(TF_BUILD_TESTS off)
set(TF_BUILD_EXAMPLES off)
set(TF_BUILD_BENCHMARKS off)
add_subdirectory(ext/taskflow)

target_link_libraries(${TARGET_NAME}
    PUBLIC Eigen3::Eigen yaml-cpp cparse Taskflow)

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})