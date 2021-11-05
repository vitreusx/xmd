file(GLOB_RECURSE PRIVATE_SOURCES CONFIGURE_DEPENDS
    src/*.cpp src/*.cxx src/*.cu
    src/*.hpp src/*.h src/*.cuh src/*.inl)

set(TARGET_NAME "fastmd")

set(CUDAToolkit_ROOT "/opt/cuda")
enable_language(CUDA)
set(CMAKE_CUDA_COMPILER "/opt/cuda/bin/nvcc")

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE ${PRIVATE_SOURCES})

target_compile_features(${TARGET_NAME}
    PUBLIC cxx_std_17)

target_include_directories(${TARGET_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_options(${TARGET_NAME}
    PRIVATE
    $<$<CONFIG:Debug>:-g;$<$<COMPILE_LANGUAGE:CXX>:-Og>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>;$<$<COMPILE_LANGUAGE:CUDA>:-O2>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

set_target_properties(${TARGET_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON)

find_package(CUDAToolkit REQUIRED)

set(Thrust_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/thrust/thrust/cmake")
set(CUB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/cub/cub/cmake")
find_package(Thrust REQUIRED CONFIG)
thrust_create_target(Thrust)

target_link_libraries(${TARGET_NAME}
    PRIVATE CUDA::cudart Thrust)