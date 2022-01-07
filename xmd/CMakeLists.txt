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
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-g>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-g;-O2;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

add_subdirectory(ext/eigen-3.4.0)
add_subdirectory(ext/yaml-cpp-0.7.0)
find_package(OpenMP REQUIRED)
add_subdirectory(ext/cparse)
add_subdirectory(ext/gentypes)
find_package(Boost REQUIRED COMPONENTS serialization)

target_link_libraries(${TARGET_NAME}
    PUBLIC Eigen3::Eigen yaml-cpp cparse OpenMP::OpenMP_CXX
    gentypes Boost::serialization)

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(docs)

add_executable(xmd_omp main.cpp)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

target_compile_options(xmd_omp
    PRIVATE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-g>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-g;-O2;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

target_link_libraries(xmd_omp PUBLIC xmd)